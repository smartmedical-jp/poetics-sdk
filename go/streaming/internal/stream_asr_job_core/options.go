package stream_asr_job_core

type CreateStreamAsrJobOptions struct {
	EnableDataLogging bool
	AudioEncoding     string `validate:"required,oneof=s16le mulaw"`
	AudioSampleRate   int    `validate:"required,oneof=8000 11000 16000 22000 32000 44100 48000"`
	ChannelCount      int    `validate:"required,oneof=1 2"`
	ConversationTitle *string
	Channels          *[]CreateStreamAsrJobOptionsChannel
}

type CreateStreamAsrJobOptionsChannel struct {
	ParticipantName string
}

type ConnectToStreamAsrJobOptions struct {
	StreamAsrJobID string `validate:"required,min=36,max=36"`
	ChannelCount   int    `validate:"required,oneof=1 2"`
}
