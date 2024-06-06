package streaming

import (
	"os"
)

type StreamingClientOptions struct {
	ApiKey   string
	EndPoint string
}

func (o StreamingClientOptions) fallback() StreamingClientOptions {
	out := o
	if out.EndPoint == "" {
		out.EndPoint = os.Getenv("POETICS_STREAMING_API_ENDPOINT")
	}
	if out.EndPoint == "" {
		out.EndPoint = "wss://stream.api.poetics-ai.com/v1"
	}
	if out.ApiKey == "" {
		out.ApiKey = os.Getenv("POETICS_API_KEY")
	}
	if out.ApiKey == "" {
		panic("API key is required")
	}

	return out
}

type StreamingClient struct {
	url string
}

func NewStreamingClientWithOptions(options StreamingClientOptions) *StreamingClient {

	opts := options.fallback()
	url := opts.EndPoint + "?api_key=" + opts.ApiKey
	return &StreamingClient{url: url}
}

func NewStreamingClient() *StreamingClient {
	return NewStreamingClientWithOptions(StreamingClientOptions{})
}

func (c *StreamingClient) CreateStreamAsrJob(options CreateStreamAsrJobOptions) (*StreamAsrJob, error) {
	return createStreamAsrJob(c.url, options)
}

func (c *StreamingClient) ConnectToStreamAsrJob(options ConnectToStreamAsrJobOptions) (*StreamAsrJob, error) {
	return connectToStreamAsrJob(c.url, options)
}
