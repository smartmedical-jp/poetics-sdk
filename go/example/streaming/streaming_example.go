package main

import (
	"log/slog"
	"os"
	"time"

	"github.com/samber/lo"
	"github.com/smartmedical-jp/poetics-sdk/go/streaming"
)

var (
	wavFileName   = "../../../testdata/long_s16le_8k.wav"
	encoding      = "s16le"
	bytePerSample = 2 // s16le の場合は 16bit=2byte
	sampleRate    = 8000
)

func main() {
	// SDK のログレベルを Debug にする
	streaming.SetDefaultLogger(slog.LevelDebug)

	// クライアントの初期化
	// 環境変数 POETICS_API_KEY に API キーが設定されていることを前提とする
	client := streaming.NewStreamingClient()

	// テスト用音声データを読み込む
	wavFile := lo.Must(os.Open(wavFileName))
	defer wavFile.Close()
	wavInfo := lo.Must(wavFile.Stat())
	audioData := make([]byte, wavInfo.Size())
	_ = lo.Must(wavFile.Read(audioData))

	// ストリーミングジョブの作成
	job, err := client.CreateStreamAsrJob(streaming.CreateStreamAsrJobOptions{
		EnableDataLogging: true,
		AudioEncoding:     encoding,
		AudioSampleRate:   sampleRate,
		ChannelCount:      1,
	})
	if err != nil {
		panic(err)
	}

	// 別スレッドで音声データをリアルタイムに投入
	go func() {
		recordingBufferMillisec := 500 // 仮想的な録音バッファの長さ（ミリ秒）
		recordingBufferSize := int(float64(sampleRate) * float64(bytePerSample) * float64(recordingBufferMillisec) / 1000)

		// リアルタイムにジョブに投入
		for i := 0; i < len(audioData); i += recordingBufferSize {
			end := i + recordingBufferSize
			if end > len(audioData) {
				end = len(audioData)
			}
			job.EnqueueAudioData(0, audioData[i:end])
			slog.Info("Enqueued Audio Data", "start", i, "end", end)
			time.Sleep(time.Duration(recordingBufferMillisec) * time.Millisecond)
		}

		// 音声データの投入完了
		job.FinishEnqueuingAudioData()
		slog.Info("Finish Enqueuing Audio Data")
	}()

	// 音声認識結果を取得しながら終了を待つ
	for u := range job.SubscribeUtterance() {
		if u.IsTemporary {
			slog.Info("Temporary Utterance", "text", u.Text)
		} else {
			slog.Info("Utterance", "text", u.Text)
		}
	}

	// ジョブのエラーがないことを確認
	if err := job.Err(); err != nil {
		panic(err)
	}

	// ジョブの詳細を表示
	jobDetail, err := job.JobDetail()
	if err != nil {
		panic(err)
	}
	slog.Info("Job Detail", "detail", jobDetail)
}
