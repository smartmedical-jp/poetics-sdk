package stream_asr_job_incoming_message

import (
	"encoding/json"
	"fmt"
)

func Parse(
	msg string,
) (interface{}, error) {

	// メッセージの型を判定する
	var msgType struct {
		Message string `json:"string"`
	}
	err := json.Unmarshal([]byte(msg), &msgType)
	if err != nil {
		return nil, err
	}

	// メッセージの型に応じてパースする
	switch msgType.Message {
	case "audio_fragment_submission_progress":
		var message AudioFragmentSubmissionProgressMessage
		err := json.Unmarshal([]byte(msg), &message)
		if err != nil {
			return nil, err
		}
		return message, nil

	case "error":
		var message ErrorMessage
		err := json.Unmarshal([]byte(msg), &message)
		if err != nil {
			return nil, err
		}
		return message, nil

	case "job_detail":
		var message JobDetailMessage
		err := json.Unmarshal([]byte(msg), &message)
		if err != nil {
			return nil, err
		}
		return message, nil

	case "job_status_updated":
		var message JobStatusUpdatedMessage
		err := json.Unmarshal([]byte(msg), &message)
		if err != nil {
			return nil, err
		}
		return message, nil

	case "temporary_utterance":
		var message TemporaryUtteranceMessage
		err := json.Unmarshal([]byte(msg), &message)
		if err != nil {
			return nil, err
		}
		return message, nil

	case "utterance":
		var message UtteranceMessage
		err := json.Unmarshal([]byte(msg), &message)
		if err != nil {
			return nil, err
		}
		return message, nil

	default:
		return nil, fmt.Errorf("unknown message type: %s", msgType.Message)
	}

}
