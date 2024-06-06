package stream_asr_job_outgoing_message

type CloseStreamAsrJobMessage struct {
	Command string `json:"command"`
}

func NewCloseStreamAsrJobMessage() CloseStreamAsrJobMessage {
	return CloseStreamAsrJobMessage{
		Command: "close_stream_asr_job",
	}
}
