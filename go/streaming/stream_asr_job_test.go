package streaming

import (
	"os"
	"strings"
	"testing"
	"time"

	"github.com/samber/lo"
)

func Test_StreamAsrJob(t *testing.T) {
	// テスト音声の読み込み
	file := lo.Must(os.Open("../../testdata/short_s16le_8k.wav"))
	defer file.Close()
	_ = lo.Must(file.Seek(44, 0))
	fileInfo := lo.Must(file.Stat())
	data := make([]byte, fileInfo.Size()-44)
	_ = lo.Must(file.Read(data))

	// ストリーミングクライアントの作成
	client := NewStreamingClient()

	// ストリーミングジョブの作成
	job, err := client.CreateStreamAsrJob(CreateStreamAsrJobOptions{
		EnableDataLogging: true,
		AudioEncoding:     "s16le",
		AudioSampleRate:   8000,
		ChannelCount:      1,
	})
	if err != nil {
		t.Fatal(err)
	}

	// 音声データの投入
	job.EnqueueAudioData(0, data)

	// 少し待つ
	for {
		time.Sleep(1 * time.Second)
		if job.core.Debug_GetNextFragmentIndices()[0] > 0 {
			break
		}
	}

	// NextFragmentIndex を書き換える（問題がないことを確認）
	indices := []int{0}
	job.core.Debug_SetNextFragmentIndices(indices)

	// WebSocket接続の切断（再接続を確認）
	job.conn.Debug_Disconnect()

	// 音声データの投入完了
	job.FinishEnqueuingAudioData()

	// 結果の取得
	result := ""
	for u := range job.SubscribeUtterance() {
		if !u.IsTemporary {
			result += u.Text
		}
	}
	// resultが"こんにちは"を含むか確認
	if !strings.Contains(result, "こんにちは") {
		t.Errorf("result does not contain 'こんにちは': %s", result)
	}

	// ジョブにエラーが無いことを確認
	if err := job.Err(); err != nil {
		t.Errorf("error: %s", err)
	}

	// ジョブの詳細を確認
	detail, err := job.JobDetail()
	if err != nil {
		t.Errorf("error: %s", err)
	}
	if detail.Status != "completed" {
		t.Errorf("unexpected job status: %s", detail.Status)
	}
}

func Test_StreamAsrJob_Stereo(t *testing.T) {
	// テスト音声の読み込み
	file := lo.Must(os.Open("../../testdata/short_s16le_8k.wav"))
	defer file.Close()
	_ = lo.Must(file.Seek(44, 0))
	fileInfo := lo.Must(file.Stat())
	data := make([]byte, fileInfo.Size()-44)
	_ = lo.Must(file.Read(data))

	// ストリーミングクライアントの作成
	client := NewStreamingClient()

	// ストリーミングジョブの作成
	job, err := client.CreateStreamAsrJob(CreateStreamAsrJobOptions{
		EnableDataLogging: true,
		AudioEncoding:     "s16le",
		AudioSampleRate:   8000,
		ChannelCount:      2,
	})
	if err != nil {
		t.Fatal(err)
	}

	// 音声データの投入（左右同じ音声を入れる）
	job.EnqueueAudioData(0, data)
	job.EnqueueAudioData(1, data)

	// 音声データの投入完了
	job.FinishEnqueuingAudioData()

	// 結果の取得
	resultCh0 := ""
	resultCh1 := ""
	for u := range job.SubscribeUtterance() {
		if !u.IsTemporary {
			if u.ChannelIndex == 0 {
				resultCh0 += u.Text
			} else if u.ChannelIndex == 1 {
				resultCh1 += u.Text
			}
		}
	}
	// resultが"こんにちは"を含むか確認
	if !strings.Contains(resultCh0, "こんにちは") {
		t.Errorf("resultCh0 does not contain 'こんにちは': %s", resultCh0)
	}
	if !strings.Contains(resultCh1, "こんにちは") {
		t.Errorf("resultCh1 does not contain 'こんにちは': %s", resultCh1)
	}

	// ジョブにエラーが無いことを確認
	if err := job.Err(); err != nil {
		t.Errorf("error: %s", err)
	}
}
