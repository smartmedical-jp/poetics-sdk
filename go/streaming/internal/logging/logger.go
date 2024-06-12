package logging

import (
	"log/slog"
	"os"

	"github.com/lmittmann/tint"
)

var Logger *slog.Logger = NewDefaultLoggerWithLevel(slog.LevelInfo)

func SetLogger(logger *slog.Logger) {
	Logger = logger
}

func NewDefaultLoggerWithLevel(level slog.Level) *slog.Logger {
	return slog.New(tint.NewHandler(os.Stdout, &tint.Options{
		Level:      level,
		TimeFormat: "2006-01-02 15:04:05",
	}))
}
