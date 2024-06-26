package stream_asr_job_core

import "time"

type StreamAsrJobDetail struct {
	StreamAsrJobID       string
	ConversationID       *string
	IsDataLoggingEnabled bool
	AudioEncoding        string
	AudioSampleRate      int
	Status               string
	ChannelCount         int
	CreatedAt            time.Time
}
