package websocket_connection

import (
	"math/rand"
	"net/http"
	"net/http/httptest"
	"strconv"
	"strings"
	"testing"
	"time"

	"github.com/gorilla/websocket"
	"github.com/smartmedical-jp/poetics-sdk/go/streaming/internal/logging"
)

// テスト用の WebSocket サーバーを立ち上げる
func createWebsocketEchoServer() (*httptest.Server, string) {
	upgrader := websocket.Upgrader{}
	s := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		conn, _ := upgrader.Upgrade(w, r, nil)
		logging.Logger.Debug("server connected")
		defer func() {
			logging.Logger.Debug("server disconnected")
			conn.Close()
		}()
		for {
			mt, message, err := conn.ReadMessage()
			if err != nil {
				return
			}
			err = conn.WriteMessage(mt, message)
			if err != nil {
				return
			}
		}
	}))
	return s, "ws" + strings.TrimPrefix(s.URL, "http")
}

func TestWebsocketConnection(t *testing.T) {
	s, url := createWebsocketEchoServer()
	defer s.Close()

	// WebSocket 接続を作成
	conn, err := NewWebsocketConnection(url)
	if err != nil {
		t.Fatal(err)
	}
	defer conn.Close()

	var testSendAndReceive = func(beforeSend func()) {
		type Msg struct {
			Text string `json:"text"`
		}

		text := strconv.Itoa(rand.Intn(100))
		message := Msg{Text: text}
		messageStr := `{"text":"` + text + `"}`

		beforeSend()

		conn.Send(message)
		select {
		case received := <-conn.SubscribeMessage():
			if received != messageStr {
				t.Errorf("unexpected message: got %s, want %s", received, message)
			}
		case <-time.After(5 * time.Second):
			t.Error("timeout waiting for message")
		}
	}

	// メッセージ送信と受信のテスト
	testSendAndReceive(func() {})
	testSendAndReceive(func() {})

	// 再接続のテスト
	reconnectCh := make(chan struct{}, 1)
	conn.SetOnReconnect(func() {
		close(reconnectCh)
	})
	testSendAndReceive(func() {
		conn.conn.Close()
	})
	select {
	case <-reconnectCh:
	case <-time.After(time.Second):
		t.Error("timeout waiting for reconnect")
	}

	// もう一回再接続のテスト
	reconnectCh = make(chan struct{}, 1)
	conn.SetOnReconnect(func() {
		close(reconnectCh)
	})
	testSendAndReceive(func() {
		conn.conn.Close()
	})
	select {
	case <-reconnectCh:
	case <-time.After(time.Second):
		t.Error("timeout waiting for reconnect")
	}

	// 正常終了のテスト
	doneCh := conn.SubscribeDone()
	conn.Close()
	select {
	case <-doneCh:
	case <-time.After(time.Second):
		t.Error("timeout waiting for done")
	}
}

func TestWebsocketConnection_ReconnectFailed(t *testing.T) {
	s, url := createWebsocketEchoServer()
	defer s.Close()

	// WebSocket 接続を作成
	conn, err := NewWebsocketConnection(url)
	if err != nil {
		t.Fatal(err)
	}
	defer conn.Close()

	// 再接続回数上限を書き換える
	N := 2
	conn.remainingReconnectAttempts = N

	// 再接続を連続で失敗させる
	reconnectCh := make(chan struct{}, 1)
	conn.SetOnReconnect(func() {
		conn.conn.Close()
		reconnectCh <- struct{}{}
	})
	conn.conn.Close()

	// reconnectCh にメッセージがN回送信されるまで待機
	for i := 0; i < N; i++ {
		select {
		case <-reconnectCh:
		case <-time.After(5 * time.Second):
			t.Error("timeout waiting for reconnect")
		}
	}

	// エラーチャンネルにエラーが送信されることを確認
	errorCh := conn.SubscribeError()
	isErrorReceived := false
	select {
	case _, ok := <-errorCh:
		if ok {
			isErrorReceived = true
		}
	case <-time.After(5 * time.Second):
		t.Error("timeout waiting for error")
	}
	if !isErrorReceived {
		t.Error("error not received")
	}

	// Done チャンネルにメッセージが送信されることを確認
	doneCh := conn.SubscribeDone()
	select {
	case <-doneCh:
	case <-time.After(time.Second):
		t.Error("timeout waiting for done")
	}
}
