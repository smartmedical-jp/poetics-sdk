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
