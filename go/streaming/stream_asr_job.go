package streaming

import (
	"fmt"
	"log/slog"

	"github.com/gorilla/websocket"
	"github.com/smartmedical-jp/poetics-sdk-go/streaming/internal/audio_buffer"
	"github.com/smartmedical-jp/poetics-sdk-go/streaming/internal/stream_asr_job_outgoing_message"
)

type StreamAsrJob struct {
	conn                     *websocket.Conn
	nextFragmentIndexes      []int
	audioBuffers             []audio_buffer.AudioBuffer
	isNextFragmentAcceptable bool
	isRecordingFinished      bool
	isCloseJobMessageSent    bool
	detail                   *StreamAsrJobDetail
	UtteranceChan            chan Utterance
	TemporaryUtteranceChan   chan TemporaryUtterance
	ErrChan                  chan error
}

func CreateStreamAsrJob(
	conn *websocket.Conn,
	enableDataLogging bool,
	audioEncoding string,
	audioSampleRate int,
	channelCount int,
	conversationTitle *string,
	channels *[]CreateStreamAsrJobMessageBodyChannel,
) (StreamAsrJob, error) {

	// create_stream_asr_job メッセージを送信する
	convTitle := ""
	if conversationTitle != nil {
		convTitle = *conversationTitle
	}

	chs := []stream_asr_job_outgoing_message.CreateStreamAsrJobMessageBodyChannel{}
	if channels != nil {
		for _, ch := range *channels {
			chs = append(chs, stream_asr_job_outgoing_message.CreateStreamAsrJobMessageBodyChannel{
				ParticipantName: ch.ParticipantName,
			})
		}
	}

	msg := stream_asr_job_outgoing_message.NewCreateStreamAsrJobMessage(
		enableDataLogging,
		convTitle,
		audioEncoding,
		audioSampleRate,
		channelCount,
		chs,
	)

	slog.Info("Sending create_stream_asr_job message...")

	err := conn.WriteJSON(msg)
	if err != nil {
		return StreamAsrJob{}, err
	}

	return newStreamAsrJob(conn, channelCount), nil
}

func newStreamAsrJob(
	conn *websocket.Conn,
	channelCount int,
) StreamAsrJob {
	// WebSocket 接続を行う

	audioBuffers := make([]audio_buffer.AudioBuffer, channelCount)
	for i := 0; i < channelCount; i++ {
		audioBuffers[i] = audio_buffer.NewAudioBuffer()
	}

	return StreamAsrJob{
		conn:                     conn,
		nextFragmentIndexes:      nil,
		audioBuffers:             audioBuffers,
		isNextFragmentAcceptable: false,
		isRecordingFinished:      false,
		isCloseJobMessageSent:    false,
		detail:                   nil,
		UtteranceChan:            make(chan Utterance, 100),
		TemporaryUtteranceChan:   make(chan TemporaryUtterance, 100),
		ErrChan:                  make(chan error, 1),
	}
}

func (j *StreamAsrJob) ProcessAudioData() {
	defer close(j.UtteranceChan)
	defer close(j.TemporaryUtteranceChan)
	defer close(j.ErrChan)

}

func (j *StreamAsrJob) PutAudioData(
	channelIndex int,
	audioData []byte,
) error {
	if (channelIndex < 0) || (channelIndex >= len(j.audioBuffers)) {
		return fmt.Errorf("invalid channel index: %d", channelIndex)
	}
	if j.isRecordingFinished {
		return fmt.Errorf("cannot send audio data because FinishPuttingAudioData has already been called")
	}
	if j.detail != nil && j.detail.Status != "open" {
		return fmt.Errorf("cannot send audio data because the job status is %s", j.detail.Status)
	}
	j.audioBuffers[channelIndex].AppendData(audioData)
	return nil
}

func (j *StreamAsrJob) FinishPuttingAudioData() error {
	if j.detail != nil && j.detail.Status != "open" {
		return fmt.Errorf("cannot finish sending audio data because the job status is already %s", j.detail.Status)
	}
	j.isRecordingFinished = true
	return nil
}
