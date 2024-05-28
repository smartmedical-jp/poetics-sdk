package stream_asr_job_incoming_message

type JobStatusUpdatedMessage struct {
	Message string                      `json:"message" validate:"eq=job_status_updated"`
	Body    JobStatusUpdatedMessageBody `json:"body" validate:"required"`
}

type JobStatusUpdatedMessageBody struct {
	Status string `json:"status" validate:"required"`
}
