package logging

import (
	"log/slog"
	"os"

	"github.com/lmittmann/tint"
)

var Logger *slog.Logger = slog.New(tint.NewHandler(os.Stdout, &tint.Options{
	Level:      slog.LevelDebug,
	TimeFormat: "2006-01-02 15:04:05",
}))

func SetLogger(logger *slog.Logger) {
	Logger = logger
}
