package streaming

type StreamingClient struct {
	apiKey string
}

func NewStreamingClient(apiKey string) (StreamingClient, error) {
	return StreamingClient{
		apiKey: apiKey,
	}, nil
}

type CreateStreamAsrJobMessageBodyChannel struct {
	ParticipantName string
}

func (c StreamingClient) CreateStreamAsrJob(
	enableDataLogging bool,
	audioEncoding string,
	audioSampleRate int,
	channelCount int,
	conversationTitle *string,
	channels *[]CreateStreamAsrJobMessageBodyChannel,
) (StreamAsrJob, error) {
	// TODO: implement
	return StreamAsrJob{}, nil
}
