package streaming

import (
	"net/http"
	"net/http/httptest"
	"strings"
	"testing"
	"time"

	"github.com/gorilla/websocket"
)

func TestWebsocketConnection(t *testing.T) {
	// テスト用のWebSocketサーバーを起動
	upgrader := websocket.Upgrader{}
	s := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		conn, _ := upgrader.Upgrade(w, r, nil)
		defer conn.Close()

		// クライアントからのメッセージをエコーバック
		for {
			_, message, err := conn.ReadMessage()
			if err != nil {
				break
			}
			conn.WriteMessage(websocket.TextMessage, message)
		}
	}))
	defer s.Close()

	u := "ws" + strings.TrimPrefix(s.URL, "http")
	wc, err := newStreamingConnection(u)
	if err != nil {
		t.Fatalf("Failed to create WebsocketConnection: %v", err)
	}
	defer wc.close()

	// メッセージ送信と受信のテスト
	type Message struct {
		Text string `json:"text"`
	}
	msg := Message{Text: "Hello, world!"}
	expected := `{"text":"Hello, world!"}`
	wc.send(msg)
	select {
	case receivedMsg := <-wc.subscribeMessage():
		if receivedMsg != expected {
			t.Errorf("Received message does not match: expected '%s', got '%s'", expected, receivedMsg)
		}
	case <-time.After(time.Second):
		t.Error("Timeout waiting for message")
	}

	// 再接続のテスト
	wc.enableAutoReconnect(3, 100*time.Millisecond, func() {}) // 自動再接続を有効化
	wc.conn.Close()                                            // 接続を強制的に切断
	wc.send(msg)                                               // 再接続後にメッセージを送信
	select {
	case receivedMsg := <-wc.subscribeMessage():
		if receivedMsg != expected {
			t.Errorf("Received message after reconnect does not match: expected '%s', got '%s'", expected, receivedMsg)
		}
	case <-time.After(time.Second):
		t.Error("Timeout waiting for message after reconnect")
	}

	// 再接続失敗のテスト
	wc.enableAutoReconnect(1, 100*time.Millisecond, func() {}) // 最大再接続回数を1に設定
	wc.conn.Close()                                            // 接続を強制的に切断
	s.Close()                                                  // サーバーを停止して再接続を失敗させる
	wc.send(msg)                                               // 再接続を試みる
	select {
	case err := <-wc.subscribeError():
		if err == nil {
			t.Error("Expected error after reconnect failure")
		}
	case <-time.After(time.Second):
		t.Error("Timeout waiting for error after reconnect failure")
	}
}
