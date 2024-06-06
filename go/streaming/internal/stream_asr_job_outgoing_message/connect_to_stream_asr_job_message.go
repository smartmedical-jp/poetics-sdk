package stream_asr_job_outgoing_message

type ConnectToStreamAsrJobMessage struct {
	Command string                        `json:"command"`
	Body    ConnectToStreamJobMessageBody `json:"body"`
}

type ConnectToStreamJobMessageBody struct {
	StreamAsrJobID string `json:"stream_asr_job_id"`
}

func NewConnectToStreamAsrJobMessage(
	streamAsrJobID string,
) ConnectToStreamAsrJobMessage {
	return ConnectToStreamAsrJobMessage{
		Command: "connect_to_stream_asr_job",
		Body: ConnectToStreamJobMessageBody{
			StreamAsrJobID: streamAsrJobID,
		},
	}
}
