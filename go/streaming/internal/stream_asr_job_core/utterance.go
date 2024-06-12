package stream_asr_job_core

import "time"

type Utterance struct {
	UtteranceID  string
	IsTemporary  bool
	ChannelIndex int
	StartAt      time.Time
	EndAt        time.Time
	Text         string
}

func NewUtterance(
	utteranceID string,
	isTemporary bool,
	channelIndex int,
	startAt time.Time,
	endAt time.Time,
	text string,
) Utterance {
	return Utterance{
		UtteranceID:  utteranceID,
		IsTemporary:  isTemporary,
		ChannelIndex: channelIndex,
		StartAt:      startAt,
		EndAt:        endAt,
		Text:         text,
	}
}
