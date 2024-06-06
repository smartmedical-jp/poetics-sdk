package stream_asr_job_outgoing_message

import "encoding/base64"

type SubmitAudioFragmentMessage struct {
	Command string                         `json:"command"`
	Body    SubmitAudioFragmentMessageBody `json:"body"`
}

type SubmitAudioFragmentMessageBody struct {
	ChannelIndex       int    `json:"channel_index"`
	AudioFragmentIndex int    `json:"audio_fragment_index"`
	Base64AudioData    string `json:"base64_audio_data"`
}

func NewSubmitAudioFragmentMessage(
	channelIndex int,
	audioFragmentIndex int,
	audioData []byte,
) SubmitAudioFragmentMessage {
	return SubmitAudioFragmentMessage{
		Command: "submit_audio_fragment",
		Body: SubmitAudioFragmentMessageBody{
			ChannelIndex:       channelIndex,
			AudioFragmentIndex: audioFragmentIndex,
			Base64AudioData:    base64.StdEncoding.EncodeToString(audioData),
		},
	}
}
