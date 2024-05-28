package streaming

import (
	"testing"
)

func TestNewStreamingConnection(t *testing.T) {
	// valid case
	conn, err := NewStreamingConnection()
	if err != nil {
		t.Fatalf("Error creating connection: %v\n", err)
	}
	conn.Close()

	// invalid api key
	_, err = NewStreamingConnectionWithOptions(StreamingConnectionOptions{
		ApiKey: "invalid",
	})
	if err == nil {
		t.Fatalf("Expected error creating connection with invalid api key\n")
	}
}
