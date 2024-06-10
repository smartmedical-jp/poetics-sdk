package stream_asr_job_core

import (
	"fmt"
	"time"

	"github.com/smartmedical-jp/poetics-sdk/go/streaming/internal/audio_buffer"
	"github.com/smartmedical-jp/poetics-sdk/go/streaming/internal/stream_asr_job_incoming_message"
	"github.com/smartmedical-jp/poetics-sdk/go/streaming/internal/stream_asr_job_outgoing_message"
)

type connectionInterface interface {
	SetOnReconnect(onReconnect func())
	Send(message interface{})
}

type StreamAsrJobCore struct {
	conn connectionInterface

	channelCount                     int
	audioBuffers                     []audio_buffer.AudioBuffer
	isNextFragmentIndicesInitialized bool
	nextFragmentIndices              []int
	isJobDetailInitialized           bool
	jobStatus                        string
	jobID                            string
	isNextFragmentSendable           bool
	isEnqueuingAudioDataFinished     bool
	isCloseJobMessageSent            bool

	onUtteranceFunc          func(Utterance)
	onTemporaryUtteranceFunc func(TemporaryUtterance)
	onErrorMessageFunc       func(error)
}

var _ StreamAsrJobCoreInterface = &StreamAsrJobCore{}

func NewStreamAsrJobCore(conn connectionInterface, channelCount int) *StreamAsrJobCore {
	audioBuffers := make([]audio_buffer.AudioBuffer, channelCount)
	for i := 0; i < channelCount; i++ {
		audioBuffers[i] = audio_buffer.NewAudioBuffer()
	}
	core := &StreamAsrJobCore{
		conn: conn,

		channelCount:                     channelCount,
		audioBuffers:                     audioBuffers,
		isNextFragmentIndicesInitialized: false,
		nextFragmentIndices:              make([]int, channelCount),
		isJobDetailInitialized:           false,
		jobStatus:                        "",
		jobID:                            "",
		isNextFragmentSendable:           false,
		isEnqueuingAudioDataFinished:     false,
		isCloseJobMessageSent:            false,

		onUtteranceFunc:          func(Utterance) {},
		onTemporaryUtteranceFunc: func(TemporaryUtterance) {},
		onErrorMessageFunc:       func(error) {},
	}

	conn.SetOnReconnect(core.onReconnect)

	return core
}

func (c *StreamAsrJobCore) CreateStreamAsrJob(options CreateStreamAsrJobOptions) {
	c.conn.Send(buildCreateStreamAsrJobMessage(options))
}

func (c *StreamAsrJobCore) ConnectToStreamAsrJob(options ConnectToStreamAsrJobOptions) {
	c.conn.Send(stream_asr_job_outgoing_message.NewConnectToStreamAsrJobMessage(options.StreamAsrJobID))
}

func (c *StreamAsrJobCore) onReconnect() {
	c.conn.Send(stream_asr_job_outgoing_message.NewConnectToStreamAsrJobMessage(c.jobID))
	c.isNextFragmentIndicesInitialized = false
	c.isNextFragmentSendable = false
}

func (c *StreamAsrJobCore) OnErrorMessage(msg stream_asr_job_incoming_message.ErrorMessage) {
	c.onErrorMessageFunc(fmt.Errorf("ignorable error message: %s", msg.Body.Message))
}

func (c *StreamAsrJobCore) OnJobDetailMessage(msg stream_asr_job_incoming_message.JobDetailMessage) {
	c.jobID = msg.Body.StreamAsrJobID
	c.jobStatus = msg.Body.Status
	c.isJobDetailInitialized = true
}

func (c *StreamAsrJobCore) OnJobStatusUpdatedMessage(msg stream_asr_job_incoming_message.JobStatusUpdatedMessage) {
	c.jobStatus = msg.Body.Status
}

func (c *StreamAsrJobCore) OnAudioFragmentSubmissionProgressMessage(msg stream_asr_job_incoming_message.AudioFragmentSubmissionProgressMessage) {
	// TODO: fix
	if len(msg.Body.Channels) == 0 {
		return
	}

	for i := 0; i < c.channelCount; i++ {
		c.nextFragmentIndices[i] = msg.Body.Channels[i].AudioFragmentCount
	}
	c.isNextFragmentIndicesInitialized = true
	c.isNextFragmentSendable = true
}

func (c *StreamAsrJobCore) OnTemporaryUtteranceMessage(msg stream_asr_job_incoming_message.TemporaryUtteranceMessage) {
	u := NewTemporaryUtterance(
		msg.Body.UtteranceID,
		msg.Body.ChannelIndex,
		msg.Body.StartAt,
		msg.Body.EndAt,
		msg.Body.Text,
	)
	c.onTemporaryUtteranceFunc(u)
}

func (c *StreamAsrJobCore) OnUtteranceMessage(msg stream_asr_job_incoming_message.UtteranceMessage) {
	u := NewUtterance(
		msg.Body.UtteranceID,
		msg.Body.ChannelIndex,
		msg.Body.StartAt,
		msg.Body.EndAt,
		msg.Body.Text,
	)
	c.onUtteranceFunc(u)
}

func (c *StreamAsrJobCore) OnReceiveMessageTimeout() {
	c.isNextFragmentSendable = true
}

func (c *StreamAsrJobCore) EnqueueAudioData(channelIndex int, data []byte) error {
	if c.isEnqueuingAudioDataFinished {
		return fmt.Errorf("finishEnqueuingAudioData() has already been called")
	}
	if channelIndex < 0 || channelIndex >= c.channelCount {
		return fmt.Errorf("invalid channel index: %d", channelIndex)
	}
	c.audioBuffers[channelIndex].AppendAudioData(data)
	return nil
}

func (c *StreamAsrJobCore) FinishEnqueuingAudioData() {
	c.isEnqueuingAudioDataFinished = true
}

func (c StreamAsrJobCore) isAllAudioDataSent() (bool, error) {
	if !c.isEnqueuingAudioDataFinished {
		return false, nil
	} else {
		sent := true
		for i := 0; i < len(c.audioBuffers); i++ {
			// nextFragmentIndex のフラグメントの取得でnilが返ってくる場合は、送信が完了している
			fragmentIndex := c.nextFragmentIndices[i]
			f, err := c.audioBuffers[i].GetFragmentAt(fragmentIndex, true)
			if err != nil {
				return false, err
			}
			if f != nil {
				sent = false
			}
		}
		return sent, nil
	}
}

func (c *StreamAsrJobCore) Step() (continueLoop bool, err error) {
	// 未初期化の場合は何もしない
	if !c.isJobDetailInitialized || !c.isNextFragmentIndicesInitialized {
		time.Sleep(100 * time.Millisecond)
		return true, nil
	}

	// 終了条件を満たしていたら終了
	if c.jobStatus == "completed" {
		return false, nil
	}
	if c.jobStatus == "error" {
		return false, fmt.Errorf("job status is error")
	}

	// 音声データ送信が完了している場合は close_stream_asr_job を送信
	isAllAudioDataSent, err := c.isAllAudioDataSent()
	if err != nil {
		return false, err
	}
	if c.jobStatus == "open" && isAllAudioDataSent && !c.isCloseJobMessageSent {
		c.conn.Send(stream_asr_job_outgoing_message.NewCloseStreamAsrJobMessage())
		c.isCloseJobMessageSent = true
	}

	// 各チャンネルについて、次のフラグメントが送信可能であれば送信する
	if c.jobStatus == "open" {
		for i := 0; i < c.channelCount; i++ {
			if c.isNextFragmentSendable {
				fragmentIndex := c.nextFragmentIndices[i]
				f, err := c.audioBuffers[i].GetFragmentAt(fragmentIndex, c.isEnqueuingAudioDataFinished)
				if err != nil {
					return false, err
				}
				if f != nil {
					c.conn.Send(stream_asr_job_outgoing_message.NewSubmitAudioFragmentMessage(i, fragmentIndex, f))
				}
				c.isNextFragmentSendable = false
			}
		}
	}

	return true, nil
}

func buildCreateStreamAsrJobMessage(
	options CreateStreamAsrJobOptions,
) stream_asr_job_outgoing_message.CreateStreamAsrJobMessage {
	convTitle := ""
	if options.ConversationTitle != nil {
		convTitle = *options.ConversationTitle
	}
	var chs *[]stream_asr_job_outgoing_message.CreateStreamAsrJobMessageBodyChannel = nil
	if options.Channels != nil {
		chs = &[]stream_asr_job_outgoing_message.CreateStreamAsrJobMessageBodyChannel{}
		for _, ch := range *options.Channels {
			*chs = append(*chs, stream_asr_job_outgoing_message.CreateStreamAsrJobMessageBodyChannel{
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

func (c *StreamAsrJobCore) SetOnUtteranceFunc(f func(Utterance)) {
	c.onUtteranceFunc = f
}

func (c *StreamAsrJobCore) SetOnTemporaryUtteranceFunc(f func(TemporaryUtterance)) {
	c.onTemporaryUtteranceFunc = f
}

func (c *StreamAsrJobCore) SetOnErrorMessageFunc(f func(error)) {
	c.onErrorMessageFunc = f
}

func (c *StreamAsrJobCore) Debug_GetNextFragmentIndices() []int {
	return c.nextFragmentIndices
}

func (c *StreamAsrJobCore) Debug_SetNextFragmentIndices(indices []int) {
	c.nextFragmentIndices = indices
}
