package streaming

import "time"

type Utterance struct {
	UtteranceID  string
	ChannelIndex int
	StartAt      time.Time
	EndAt        time.Time
	Text         string
}
