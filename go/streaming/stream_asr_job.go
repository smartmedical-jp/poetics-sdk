package streaming

import (
	"fmt"
	"log/slog"
	"time"

	"github.com/smartmedical-jp/poetics-sdk-go/streaming/internal/audio_buffer"
	"github.com/smartmedical-jp/poetics-sdk-go/streaming/internal/stream_asr_job_incoming_message"
	"github.com/smartmedical-jp/poetics-sdk-go/streaming/internal/stream_asr_job_outgoing_message"
)

const (
	websocketMaxReconnectCount = 10
	websocketReconnectDelay    = 1 * time.Second
	receiveMessageTimeout      = 5 * time.Second
)

type StreamAsrJob struct {
	conn *StreamingConnection
	// variables
	nextFragmentIndices          *[]int
	audioBuffers                 []audio_buffer.AudioBuffer
	isNextFragmentSendable       bool
	isEnqueuingAudioDataFinished bool
	isCloseJobMessageSent        bool
	jobStatus                    *string
	jobID                        *string
	// output channels
	utteranceChan          chan Utterance
	temporaryUtteranceChan chan TemporaryUtterance
	errorChan              chan error
}

type CreateStreamAsrJobOptions struct {
	EnableDataLogging bool
	AudioEncoding     string `validate:"required,oneof=s16le mulaw"`
	AudioSampleRate   int    `validate:"required,oneof=8000 11000 16000 22000 32000 44100 48000"`
	ChannelCount      int    `validate:"required,oneof=1 2"`
	ConversationTitle *string
	Channels          *[]CreateStreamAsrJobOptionsChannel
}

type CreateStreamAsrJobOptionsChannel struct {
	ParticipantName string
}

type ConnectToStreamAsrJobOptions struct {
	StreamAsrJobID string `validate:"required,min=36,max=36"`
	ChannelCount   int    `validate:"required,oneof=1 2"`
}

func buildCreateStreamAsrJobMessage(
	options CreateStreamAsrJobOptions,
) stream_asr_job_outgoing_message.CreateStreamAsrJobMessage {
	convTitle := ""
	if options.ConversationTitle != nil {
		convTitle = *options.ConversationTitle
	}
	chs := []stream_asr_job_outgoing_message.CreateStreamAsrJobMessageBodyChannel{}
	if options.Channels != nil {
		for _, ch := range *options.Channels {
			chs = append(chs, stream_asr_job_outgoing_message.CreateStreamAsrJobMessageBodyChannel{
				ParticipantName: ch.ParticipantName,
			})
		}
	}
	msg := stream_asr_job_outgoing_message.NewCreateStreamAsrJobMessage(
		options.EnableDataLogging,
		convTitle,
		options.AudioEncoding,
		options.AudioSampleRate,
		options.ChannelCount,
		chs,
	)
	return msg
}

func CreateStreamAsrJob(
	conn *StreamingConnection,
	options CreateStreamAsrJobOptions,
) StreamAsrJob {

	// create_stream_asr_job メッセージを送信する
	msg := buildCreateStreamAsrJobMessage(options)
	conn.send(msg)

	return newStreamAsrJob(conn, options.ChannelCount)
}

func ConnectToStreamAsrJob(
	conn *StreamingConnection,
	options ConnectToStreamAsrJobOptions,
) StreamAsrJob {

	// connect_to_stream_asr_job メッセージを送信する
	msg := stream_asr_job_outgoing_message.NewConnectToStreamAsrJobMessage(options.StreamAsrJobID)
	conn.send(msg)

	return newStreamAsrJob(conn, options.ChannelCount)
}

func newStreamAsrJob(
	conn *StreamingConnection,
	channelCount int,
) StreamAsrJob {

	audioBuffers := make([]audio_buffer.AudioBuffer, channelCount)
	for i := 0; i < channelCount; i++ {
		audioBuffers[i] = audio_buffer.NewAudioBuffer()
	}

	job := StreamAsrJob{
		conn: conn,

		nextFragmentIndices:          nil,
		audioBuffers:                 audioBuffers,
		isNextFragmentSendable:       false,
		isEnqueuingAudioDataFinished: false,
		isCloseJobMessageSent:        false,
		jobStatus:                    nil,
		jobID:                        nil,

		utteranceChan:          make(chan Utterance, 100),
		temporaryUtteranceChan: make(chan TemporaryUtterance, 100),
		errorChan:              make(chan error, 1),
	}

	go job.run()

	return job
}

func (j StreamAsrJob) isAllAudioDataSent() bool {
	if !j.isEnqueuingAudioDataFinished {
		return false
	} else {
		for i := 0; i < len(j.audioBuffers); i++ {
			// nextFragmentIndex のフラグメントの取得でエラーになる場合は、音声データは送信完了している
			fragmentIndex := (*j.nextFragmentIndices)[i]
			_, err := j.audioBuffers[i].GetFragmentAt(fragmentIndex, true)
			if err == nil {
				return false
			}
		}
		return true
	}
}

func (j *StreamAsrJob) processMessage(msg string) error {
	parsed, err := stream_asr_job_incoming_message.Parse(msg)
	if err != nil {
		return err
	}

	switch msg := parsed.(type) {
	case stream_asr_job_incoming_message.ErrorMessage:
		slog.Warn("ignoring error message", "error", msg.Body.Message)

	case stream_asr_job_incoming_message.JobDetailMessage:
		j.jobID = &msg.Body.StreamAsrJobID
		j.jobStatus = &msg.Body.Status

	case stream_asr_job_incoming_message.JobStatusUpdatedMessage:
		j.jobStatus = &msg.Body.Status

	case stream_asr_job_incoming_message.AudioFragmentSubmissionProgressMessage:
		for i := 0; i < len(j.audioBuffers); i++ {
			(*j.nextFragmentIndices)[i] = msg.Body.Channels[i].AudioFragmentCount
		}
		j.isNextFragmentSendable = true

	case stream_asr_job_incoming_message.TemporaryUtteranceMessage:
		j.temporaryUtteranceChan <- NewTemporaryUtterance(
			msg.Body.UtteranceID,
			msg.Body.ChannelIndex,
			msg.Body.StartAt,
			msg.Body.EndAt,
			msg.Body.Text,
		)

	case stream_asr_job_incoming_message.UtteranceMessage:
		j.utteranceChan <- NewUtterance(
			msg.Body.UtteranceID,
			msg.Body.ChannelIndex,
			msg.Body.StartAt,
			msg.Body.EndAt,
			msg.Body.Text,
		)

	default:
		return fmt.Errorf("unexpected message type: %s", msg)
	}

	return nil
}

func (j *StreamAsrJob) run() {
	defer close(j.utteranceChan)
	defer close(j.temporaryUtteranceChan)
	defer close(j.errorChan)

	// Websocketの自動再接続を有効化
	j.conn.enableAutoReconnect(websocketMaxReconnectCount, websocketReconnectDelay, func() {
		msg := stream_asr_job_outgoing_message.NewConnectToStreamAsrJobMessage(*j.jobID)
		j.conn.send(msg)
	})

	for {

		// 初期化完了までは何もしない
		if j.jobID == nil || j.nextFragmentIndices == nil || j.jobStatus == nil {
			time.Sleep(100 * time.Millisecond)
			continue
		}

		// 音声データ送信が完了している場合は、close_stream_asr_job メッセージを送信する
		if j.isAllAudioDataSent() && !j.isCloseJobMessageSent {
			msg := stream_asr_job_outgoing_message.NewCloseStreamAsrJobMessage()
			j.conn.send(msg)
			j.isCloseJobMessageSent = true
		}

		// 各チャンネルについて、次のフラグメントが送信可能であれば送信する
		for i := 0; i < len(j.audioBuffers); i++ {
			if j.isNextFragmentSendable {
				fragmentIndex := (*j.nextFragmentIndices)[i]
				fragment, err := j.audioBuffers[i].GetFragmentAt(fragmentIndex, j.isEnqueuingAudioDataFinished)
				if err != nil {
					j.errorChan <- err
					return
				}
				if fragment != nil {
					msg := stream_asr_job_outgoing_message.NewSubmitAudioFragmentMessage(i, fragmentIndex, fragment)
					j.conn.send(msg)
				}
				j.isNextFragmentSendable = false
			}
		}

		// メッセージを受信する
		select {
		case msg := <-j.conn.subscribeMessage():
			if msg != "" {
				err := j.processMessage(msg)
				if err != nil {
					j.errorChan <- err
					return
				}
			}

		case err := <-j.conn.subscribeError():
			// 再接続エラーや正常な切断の場合はジョブを終了
			j.errorChan <- err
			return

		case <-time.After(receiveMessageTimeout):
			j.isNextFragmentSendable = true
			continue
		}

	}
}

func (j *StreamAsrJob) EnqueueAudioData(channelIndex int, audioData []byte) error {
	if j.isEnqueuingAudioDataFinished {
		return fmt.Errorf("FinishEnqueuingAudioData() has already been called")
	}
	j.audioBuffers[channelIndex].AppendAudioData(audioData)
	return nil
}

func (j *StreamAsrJob) FinishEnqueuingAudioData() {
	j.isEnqueuingAudioDataFinished = true
}

func (j *StreamAsrJob) SubscribeUtterance() <-chan Utterance {
	return j.utteranceChan
}

func (j *StreamAsrJob) SubscribeTemporaryUtterance() <-chan TemporaryUtterance {
	return j.temporaryUtteranceChan
}

func (j *StreamAsrJob) SubscribeError() <-chan error {
	return j.errorChan
}
