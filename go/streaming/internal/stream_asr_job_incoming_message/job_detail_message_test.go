package stream_asr_job_incoming_message

import (
	"encoding/json"
	"testing"
)

func TestJobDetailMessage(t *testing.T) {
	// test json string
	input := `
	{
		"message": "job_detail",
		"body": {
			"stream_asr_job_id": "edc030c2-699e-4252-9627-c431fb69acba",
			"is_data_logging_enabled": true,
			"audio_encoding": "mulaw",
			"audio_sample_rate": 8000,
			"status": "open",
			"conversation_id": "71873b2d-5399-4d1a-96b1-bb5e9f26fa51",
			"channel_count": 2,
			"created_at": "2014-10-10T13:50:40+09:00"
		}
	}`

	// test unmarshal
	var output JobDetailMessage
	err := json.Unmarshal([]byte(input), &output)
	if err != nil {
		t.Fatal(err)
	}

	// validate
	if output.Message != "job_detail" {
		t.Errorf("unexpected Message: %s", output.Message)
	}
	if output.Body.StreamAsrJobID != "edc030c2-699e-4252-9627-c431fb69acba" {
		t.Errorf("unexpected StreamAsrJobID: %s", output.Body.StreamAsrJobID)
	}
}
