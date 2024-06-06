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
	file := lo.Must(os.Open("../../testdata/test_s16le_8k.wav"))
	defer file.Close()
	fileInfo := lo.Must(file.Stat())
	data := make([]byte, fileInfo.Size())
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

	// 音声データの投入完了
	job.FinishEnqueuingAudioData()

	// 結果の取得
	result := ""
	u := <-job.SubscribeUtterance()
	result = u.Text
	// resultが"こんにちは"を含むか確認
	if !strings.Contains(result, "こんにちは") {
		t.Errorf("result does not contain 'こんにちは': %s", result)
	}

	// ジョブにエラーが無いことを確認
	err = <-job.SubscribeError()
	if err != nil {
		t.Errorf("error: %s", err)
	}

	// ジョブの終了を確認
	select {
	case <-job.SubscribeDone():
	case <-time.After(time.Second):
		t.Error("job did not finish")
	}
}
