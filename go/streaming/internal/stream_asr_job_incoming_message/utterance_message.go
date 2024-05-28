package stream_asr_job_incoming_message

import "time"

type UtteranceMessage struct {
	Message string               `json:"message" validate:"eq=utterance"`
	Body    UtteranceMessageBody `json:"body" validate:"required"`
}

type UtteranceMessageBody struct {
	ChannelIndex int       `json:"channel_index" validate:"required"`
	UtteranceID  string    `json:"utterance_id" validate:"required"`
	StartAt      time.Time `json:"start_at" validate:"required"`
	EndAt        time.Time `json:"end_at" validate:"required"`
	Text         string    `json:"text" validate:"required"`
}
