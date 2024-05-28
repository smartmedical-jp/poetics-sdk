package streaming

import (
	"fmt"
	"log/slog"
	"os"

	"github.com/gorilla/websocket"
)

type StreamingConnectionOptions struct {
	ApiKey   string
	EndPoint string
}

func (o StreamingConnectionOptions) fallback() StreamingConnectionOptions {
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

func NewStreamingConnection() (*websocket.Conn, error) {
	return NewStreamingConnectionWithOptions(StreamingConnectionOptions{})
}

func NewStreamingConnectionWithOptions(
	options StreamingConnectionOptions,
) (*websocket.Conn, error) {
	opts := options.fallback()

	url := fmt.Sprintf("%s?api_key=%s", opts.EndPoint, opts.ApiKey)
	conn, _, err := websocket.DefaultDialer.Dial(url, nil)
	if err != nil {
		return nil, err
	}

	slog.Info("Connected to streaming API.")
	return conn, nil
}
