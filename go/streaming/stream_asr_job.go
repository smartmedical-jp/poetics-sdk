package streaming

import (
	"fmt"
	"log/slog"
	"time"

	"github.com/smartmedical-jp/poetics-sdk/go/streaming/internal/logging"
	"github.com/smartmedical-jp/poetics-sdk/go/streaming/internal/stream_asr_job_core"
	"github.com/smartmedical-jp/poetics-sdk/go/streaming/internal/stream_asr_job_incoming_message"
	"github.com/smartmedical-jp/poetics-sdk/go/streaming/internal/websocket_connection"
)

type StreamAsrJob struct {
	conn *websocket_connection.WebsocketConnection
	core *stream_asr_job_core.StreamAsrJobCore

	utteranceChan chan stream_asr_job_core.Utterance
	err           error
}

func newStreamAsrJobWithInitFunc(
	url string,
	channelCount int,
	init func(*stream_asr_job_core.StreamAsrJobCore),
) (*StreamAsrJob, error) {

	conn, err := websocket_connection.NewWebsocketConnection(url)
	if err != nil {
		return nil, err
	}

	core := stream_asr_job_core.NewStreamAsrJobCore(conn, channelCount)

	job := &StreamAsrJob{
		conn:          conn,
		core:          core,
		utteranceChan: make(chan stream_asr_job_core.Utterance, 100),
		err:           nil,
	}

	core.SetOnUtteranceFunc(func(utterance Utterance) {
		logging.Logger.Debug("UTTERANCE", "is_temporary", utterance.IsTemporary, "text", utterance.Text)
		job.utteranceChan <- utterance
	})

	init(job.core)

	go job.run()

	return job, nil
}

func createStreamAsrJob(
	url string,
	options CreateStreamAsrJobOptions,
) (*StreamAsrJob, error) {
	init := func(core *stream_asr_job_core.StreamAsrJobCore) {
		core.CreateStreamAsrJob(options)
	}
	return newStreamAsrJobWithInitFunc(url, options.ChannelCount, init)
}

func connectToStreamAsrJob(
	url string,
	options ConnectToStreamAsrJobOptions,
) (*StreamAsrJob, error) {
	init := func(core *stream_asr_job_core.StreamAsrJobCore) {
		core.ConnectToStreamAsrJob(options)
	}
	return newStreamAsrJobWithInitFunc(url, 0, init)
}

func (j *StreamAsrJob) processMessage(msgStr string) error {
	parsed, err := stream_asr_job_incoming_message.Parse(msgStr)
	if err != nil {
		// メッセージのパースに失敗した場合は無視する
		slog.Warn("failed to parse message", "error", err)
		return nil
	}

	switch msg := parsed.(type) {
	case stream_asr_job_incoming_message.ErrorMessage:
		j.core.OnErrorMessage(msg)

	case stream_asr_job_incoming_message.JobDetailMessage:
		j.core.OnJobDetailMessage(msg)

	case stream_asr_job_incoming_message.JobStatusUpdatedMessage:
		j.core.OnJobStatusUpdatedMessage(msg)

	case stream_asr_job_incoming_message.AudioFragmentSubmissionProgressMessage:
		j.core.OnAudioFragmentSubmissionProgressMessage(msg)

	case stream_asr_job_incoming_message.TemporaryUtteranceMessage:
		j.core.OnTemporaryUtteranceMessage(msg)

	case stream_asr_job_incoming_message.UtteranceMessage:
		j.core.OnUtteranceMessage(msg)

	default:
		return fmt.Errorf("message of unexpected type: %s", msg)
	}

	return nil
}

func (j *StreamAsrJob) run() {
	defer j.conn.Close()
	defer close(j.utteranceChan)

	for {
		// 送信
		doContinue, err := j.core.Step()
		if err != nil {
			j.err = err
			return
		}
		if !doContinue {
			return
		}

		// 受信
		select {
		case <-j.conn.SubscribeDone():
			return

		case err := <-j.conn.SubscribeError():
			j.err = err
			return

		case msg := <-j.conn.SubscribeMessage():
			if err := j.processMessage(msg); err != nil {
				j.err = err
				return
			}

		case <-time.After(3 * time.Second):
			j.core.OnReceiveMessageTimeout()
		}
	}
}

func (j *StreamAsrJob) SubscribeUtterance() <-chan Utterance {
	return j.utteranceChan
}

func (j *StreamAsrJob) EnqueueAudioData(channelIndex int, audioData []byte) error {
	return j.core.EnqueueAudioData(channelIndex, audioData)
}

func (j *StreamAsrJob) FinishEnqueuingAudioData() {
	j.core.FinishEnqueuingAudioData()
}

func (j *StreamAsrJob) JobDetail() (StreamAsrJobDetail, error) {
	return j.core.JobDetail()
}

func (j *StreamAsrJob) Err() error {
	return j.err
}
