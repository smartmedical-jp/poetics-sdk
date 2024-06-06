package main

import (
	"fmt"
	"os"
	"time"

	"github.com/smartmedical-jp/poetics-sdk/go/streaming"
)

func main() {

	client := streaming.NewStreamingClient()

	job, err := client.CreateStreamAsrJob(streaming.CreateStreamAsrJobOptions{
		EnableDataLogging: true,
		AudioEncoding:     "s16le",
		AudioSampleRate:   8000,
		ChannelCount:      1,
	})
	if err != nil {
		panic(err)
	}

	// WAVファイルを開く
	file, err := os.Open("../testdata/test_s16le_8k.wav") // ファイル名を指定
	if err != nil {
		fmt.Println("Error opening file:", err)
		return
	}
	defer file.Close()

	// ファイル情報を取得
	fileInfo, err := file.Stat()
	if err != nil {
		fmt.Println("Error getting file info:", err)
		return
	}

	// バイトスライスを確保
	data := make([]byte, fileInfo.Size())

	// ファイルを読み込む
	_, err = file.Read(data)
	if err != nil {
		fmt.Println("Error reading file:", err)
		return
	}

	// 読み込んだデータを投入
	job.EnqueueAudioData(0, data)

	time.Sleep(3 * time.Second)

	job.FinishEnqueuingAudioData()

	select {
	case u := <-job.SubscribeUtterance():
		println(u.Text)
	case u := <-job.SubscribeTemporaryUtterance():
		println(u.Text)
	}
	select {
	case err := <-job.SubscribeError():
		if err != nil {
			println(err.Error())
		}
	case <-job.SubscribeDone():
		println("done")
	}
}
