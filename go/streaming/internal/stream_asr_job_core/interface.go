package stream_asr_job_core

// このコア部分を可能な限り言語間で共通化する

import (
	"github.com/smartmedical-jp/poetics-sdk/go/streaming/internal/stream_asr_job_incoming_message"
)

type StreamAsrJobCoreInterface interface {
	CreateStreamAsrJob(options CreateStreamAsrJobOptions)
	ConnectToStreamAsrJob(options ConnectToStreamAsrJobOptions)

	OnErrorMessage(msg stream_asr_job_incoming_message.ErrorMessage)
	OnJobDetailMessage(msg stream_asr_job_incoming_message.JobDetailMessage)
	OnJobStatusUpdatedMessage(msg stream_asr_job_incoming_message.JobStatusUpdatedMessage)
	OnAudioFragmentSubmissionProgressMessage(msg stream_asr_job_incoming_message.AudioFragmentSubmissionProgressMessage)
	OnTemporaryUtteranceMessage(msg stream_asr_job_incoming_message.TemporaryUtteranceMessage)
	OnUtteranceMessage(msg stream_asr_job_incoming_message.UtteranceMessage)
	OnReceiveMessageTimeout()

	SetOnUtteranceFunc(func(Utterance))
	SetOnErrorMessageFunc(func(error))

	EnqueueAudioData(channelIndex int, data []byte) error
	FinishEnqueuingAudioData()

	Step() (continueLoop bool, err error)
	JobDetail() (StreamAsrJobDetail, error)
}
