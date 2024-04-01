package poetics

import (
	"context"
	"os"
	"testing"
)

var apiKey = os.Getenv("POETICS_API_KEY")

func TestNewRestClientWithResponses(t *testing.T) {

	ctx := context.Background()

	client, err := NewRestClientWithResponses(apiKey)
	if err != nil {
		t.Fatalf("Error creating client: %v\n", err)
	}

	// health
	_, err = client.GetHealthWithResponse(ctx)
	if err != nil {
		t.Fatalf("Error getting health: %v\n", err)
	}

	// conversations
	_, err = client.GetConversationsWithResponse(ctx, &GetConversationsParams{
		Limit:  10,
		Offset: 0,
		Order:  "desc",
	})
	if err != nil {
		t.Fatalf("Error getting conversations: %v\n", err)
	}

	// stream_asr_jobs
	_, err = client.GetStreamAsrJobsWithResponse(ctx, &GetStreamAsrJobsParams{
		Limit:  10,
		Offset: 0,
		Order:  "asc",
	})
	if err != nil {
		t.Fatalf("Error getting stream_asr_jobs: %v\n", err)
	}
}
