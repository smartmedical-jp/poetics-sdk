package streaming

import (
	"time"
)

type TemporaryUtterance struct {
	UtteranceID  string
	ChannelIndex int
	StartAt      time.Time
	EndAt        time.Time
	Text         string
}

func NewTemporaryUtterance(
	utteranceID string,
	channelIndex int,
	startAt time.Time,
	endAt time.Time,
	text string,
) TemporaryUtterance {
	return TemporaryUtterance{
		UtteranceID:  utteranceID,
		ChannelIndex: channelIndex,
		StartAt:      startAt,
		EndAt:        endAt,
		Text:         text,
	}
}
