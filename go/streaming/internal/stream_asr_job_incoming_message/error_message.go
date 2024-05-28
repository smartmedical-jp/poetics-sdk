package stream_asr_job_incoming_message

type ErrorMessage struct {
	Message string           `json:"message" validate:"eq=error"`
	Body    ErrorMessageBody `json:"body" validate:"required"`
}

type ErrorMessageBody struct {
	Code    string `json:"code" validate:"required"`
	Message string `json:"message" validate:"required"`
}
