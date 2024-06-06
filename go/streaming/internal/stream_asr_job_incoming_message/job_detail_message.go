package stream_asr_job_incoming_message

import "time"

type JobDetailMessage struct {
	Message string               `json:"message" validate:"eq=job_detail"`
	Body    JobDetailMessageBody `json:"body" validate:"required"`
}

type JobDetailMessageBody struct {
	StreamAsrJobID       string    `json:"stream_asr_job_id" validate:"required"`
	ConversationID       *string   `json:"conversation_id"`
	IsDataLoggingEnabled bool      `json:"is_data_logging_enabled" validate:"required"`
	AudioEncoding        string    `json:"audio_encoding" validate:"required"`
	AudioSampleRate      int       `json:"audio_sample_rate" validate:"required"`
	Status               string    `json:"status" validate:"required"`
	ChannelCount         int       `json:"channel_count" validate:"required"`
	CreatedAt            time.Time `json:"created_at" validate:"required"`
}

func Hoge() {
}
