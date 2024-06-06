package stream_asr_job_incoming_message

type AudioFragmentSubmissionProgressMessage struct {
	Message string                                     `json:"message" validate:"eq=audio_fragment_submission_progress"`
	Body    AudioFragmentSubmissionProgressMessageBody `json:"body" validate:"required"`
}

type AudioFragmentSubmissionProgressMessageBody struct {
	Channels []AudioFragmentSubmissionProgressMessageBodyChannel `json:"channels"`
}

type AudioFragmentSubmissionProgressMessageBodyChannel struct {
	ChannelIndex       int `json:"channel_index" validate:"required"`
	AudioFragmentCount int `json:"audio_fragment_count" validate:"required"`
}
