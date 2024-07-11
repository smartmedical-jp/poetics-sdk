package logging

import (
	"fmt"
	"log/slog"
	"os"

	"github.com/lmittmann/tint"
)

var Logger *slog.Logger = newLogger()

func SetLogger(logger *slog.Logger) {
	Logger = logger
}

func newLogger() *slog.Logger {
	level := slog.LevelInfo
	if os.Getenv("POETICS_SDK_LOG_LEVEL") != "" {
		l := os.Getenv("POETICS_SDK_LOG_LEVEL")
		err := level.UnmarshalText([]byte(l))
		if err != nil {
			fmt.Printf("invalid log level: %s. using info level\n", l)
			level = slog.LevelInfo
		}
	}
	return NewDefaultLoggerWithLevel(level)
}

func NewDefaultLoggerWithLevel(level slog.Level) *slog.Logger {
	return slog.New(tint.NewHandler(os.Stdout, &tint.Options{
		Level:      level,
		TimeFormat: "2006-01-02 15:04:05",
	}))
}
