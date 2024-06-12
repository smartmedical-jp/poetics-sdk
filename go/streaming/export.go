package streaming

import (
	"github.com/smartmedical-jp/poetics-sdk/go/streaming/internal/logging"
	"github.com/smartmedical-jp/poetics-sdk/go/streaming/internal/stream_asr_job_core"
)

var SetLogger = logging.SetLogger
var NewDefaultLoggerWithLevel = logging.NewDefaultLoggerWithLevel

type CreateStreamAsrJobOptions = stream_asr_job_core.CreateStreamAsrJobOptions
type CreateStreamAsrJobOptionsChannel = stream_asr_job_core.CreateStreamAsrJobOptionsChannel
type ConnectToStreamAsrJobOptions = stream_asr_job_core.ConnectToStreamAsrJobOptions
type Utterance = stream_asr_job_core.Utterance
type StreamAsrJobDetail = stream_asr_job_core.StreamAsrJobDetail
