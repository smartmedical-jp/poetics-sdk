package stream_asr_job_outgoing_message

type CreateStreamAsrJobMessage struct {
	Command string                        `json:"command"`
	Body    CreateStreamAsrJobMessageBody `json:"body"`
}

type CreateStreamAsrJobMessageBody struct {
	EnableDataLogging bool                                    `json:"enable_data_logging"`
	ConversationTitle string                                  `json:"conversation_title"`
	AudioEncoding     string                                  `json:"audio_encoding"`
	AudioSampleRate   int                                     `json:"audio_sample_rate"`
	ChannelCount      int                                     `json:"channel_count"`
	Channels          *[]CreateStreamAsrJobMessageBodyChannel `json:"channels"`
}

type CreateStreamAsrJobMessageBodyChannel struct {
	ParticipantName string `json:"participant_name"`
}

func NewCreateStreamAsrJobMessage(
	enableDataLogging bool,
	conversationTitle string,
	audioEncoding string,
	audioSampleRate int,
	channelCount int,
	channels *[]CreateStreamAsrJobMessageBodyChannel,
) CreateStreamAsrJobMessage {
	return CreateStreamAsrJobMessage{
		Command: "create_stream_asr_job",
		Body: CreateStreamAsrJobMessageBody{
			EnableDataLogging: enableDataLogging,
			ConversationTitle: conversationTitle,
			AudioEncoding:     audioEncoding,
			AudioSampleRate:   audioSampleRate,
			ChannelCount:      channelCount,
			Channels:          channels,
		},
	}
}
