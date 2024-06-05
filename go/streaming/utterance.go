package streaming

import "time"

type Utterance struct {
	UtteranceID  string
	ChannelIndex int
	StartAt      time.Time
	EndAt        time.Time
	Text         string
}

func NewUtterance(
	utteranceID string,
	channelIndex int,
	startAt time.Time,
	endAt time.Time,
	text string,
) Utterance {
	return Utterance{
		UtteranceID:  utteranceID,
		ChannelIndex: channelIndex,
		StartAt:      startAt,
		EndAt:        endAt,
		Text:         text,
	}
}
