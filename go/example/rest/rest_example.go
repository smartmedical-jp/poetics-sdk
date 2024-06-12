package main

import (
	"context"
	"log/slog"
	"time"

	"github.com/smartmedical-jp/poetics-sdk/go/rest"
)

func main() {

	ctx := context.Background()

	// クライアントの初期化
	// 環境変数 POETICS_API_KEY に API キーが設定されていることを前提とする
	client, err := rest.NewRestClient()
	if err != nil {
		panic(err)
	}

	// 直近 24 時間以内に作成されたジョブの一覧を取得
	minCreatedAt := time.Now().Add(-24 * time.Hour)
	maxCreatedAt := time.Now()
	jobsResponse, err := client.GetStreamAsrJobsWithResponse(ctx, &rest.GetStreamAsrJobsParams{
		MinCreatedAt: &minCreatedAt,
		MaxCreatedAt: &maxCreatedAt,
		Limit:        100,
		Offset:       0,
		Order:        "desc",
	})
	if err != nil {
		panic(err)
	}
	slog.Info("Jobs", "count", len(*jobsResponse.JSON200))
	for _, job := range *jobsResponse.JSON200 {
		slog.Info("Job", "id", job.Id, "status", job.Status, "createdAt", job.CreatedAt)
	}
}
