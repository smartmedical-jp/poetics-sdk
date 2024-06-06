package rest

import (
	"context"
	"testing"
)

func TestNewRestClientWithResponses(t *testing.T) {

	ctx := context.Background()

	client, err := NewRestClient()
	if err != nil {
		t.Fatalf("Error creating client: %v\n", err)
	}

	// health
	healthResponse, err := client.GetHealthWithResponse(ctx)
	if err != nil || healthResponse.StatusCode() != 200 {
		t.Fatalf("Error getting health: %v, %v\n", err, healthResponse.Status())
	}

	// conversations with invalid api key
	invalidClient, err := NewRestClientWithOptions(RestClientOptions{
		ApiKey: "invalid",
	})
	if err != nil {
		t.Fatalf("Error creating client: %v\n", err)
	}
	conversationsResponse, err := invalidClient.GetConversationsWithResponse(ctx, &GetConversationsParams{
		Limit:  10,
		Offset: 0,
		Order:  "desc",
	})
	if err == nil && conversationsResponse.StatusCode() == 200 {
		t.Fatalf("Expected error getting conversations with invalid api key\n")
	}

	// conversations
	conversationsResponse, err = client.GetConversationsWithResponse(ctx, &GetConversationsParams{
		Limit:  10,
		Offset: 0,
		Order:  "desc",
	})
	if err != nil || conversationsResponse.StatusCode() != 200 {
		t.Fatalf("Error getting conversatins: %v, %v\n", err, conversationsResponse.Status())
	}

	// stream_asr_jobs
	jobsResponse, err := client.GetStreamAsrJobsWithResponse(ctx, &GetStreamAsrJobsParams{
		Limit:  10,
		Offset: 0,
		Order:  "asc",
	})
	if err != nil || jobsResponse.StatusCode() != 200 {
		t.Fatalf("Error getting stream_asr_jobs: %v, %v\n", err, jobsResponse.Status())
	}

}
