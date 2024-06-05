package streaming

// https://g.co/gemini/share/291b02e6e5f1

import (
	"encoding/json"
	"os"
	"sync"
	"time"

	"github.com/gorilla/websocket"
)

type StreamingConnection struct {
	connMu            sync.Mutex
	conn              *websocket.Conn
	url               string
	receiveChan       chan string
	errorChan         chan error
	sendBuffer        []interface{}
	sendBufferMutex   sync.Mutex
	maxReconnectCount int
	reconnectDelay    time.Duration
	reconnectCount    int
	onReconnect       func()
	closed            bool
}

type StreamingConnectionOptions struct {
	ApiKey   string
	EndPoint string
}

func (o StreamingConnectionOptions) fallback() StreamingConnectionOptions {
	out := o
	if out.EndPoint == "" {
		out.EndPoint = os.Getenv("POETICS_STREAMING_API_ENDPOINT")
	}
	if out.EndPoint == "" {
		out.EndPoint = "https://stream.api.poetics-ai.com/v1"
	}
	if out.ApiKey == "" {
		out.ApiKey = os.Getenv("POETICS_API_KEY")
	}
	if out.ApiKey == "" {
		panic("API key is required")
	}

	return out
}

func NewStreamingConnectionWithOptions(options StreamingConnectionOptions) (*StreamingConnection, error) {
	opts := options.fallback()
	url := opts.EndPoint + "?api_key=" + opts.ApiKey
	return newStreamingConnection(url)
}

func NewStreamingConnection() (*StreamingConnection, error) {
	return NewStreamingConnectionWithOptions(StreamingConnectionOptions{})
}

func newStreamingConnection(url string) (*StreamingConnection, error) {
	conn, _, err := websocket.DefaultDialer.Dial(url, nil)
	if err != nil {
		return nil, err
	}

	wc := &StreamingConnection{
		conn:              conn,
		url:               url,
		receiveChan:       make(chan string, 100),
		errorChan:         make(chan error, 1), // エラーメッセージをバッファリング
		maxReconnectCount: 0,
		reconnectDelay:    0,
		reconnectCount:    0,
		onReconnect:       func() {}, // デフォルトでは空の関数
		closed:            false,
	}
	go wc.run()
	return wc, nil
}

func (wc *StreamingConnection) enableAutoReconnect(maxReconnectCount int, delay time.Duration, onReconnect func()) {
	wc.maxReconnectCount = maxReconnectCount
	wc.reconnectDelay = delay
	wc.onReconnect = onReconnect
}

func (wc *StreamingConnection) send(msg interface{}) {
	wc.sendBufferMutex.Lock()
	wc.sendBuffer = append(wc.sendBuffer, msg)
	wc.sendBufferMutex.Unlock()
}

func (wc *StreamingConnection) subscribeMessage() <-chan string {
	return wc.receiveChan
}

func (wc *StreamingConnection) subscribeError() <-chan error {
	return wc.errorChan
}

func (wc *StreamingConnection) close() {
	if wc.closed {
		return
	}
	wc.connMu.Lock()
	wc.conn.Close()
	wc.connMu.Unlock()
	close(wc.receiveChan)
	close(wc.errorChan)
	wc.closed = true
}

func (wc *StreamingConnection) run() {
	reconnectCount := 0
	defer wc.close()

	for {
		for _, msg := range wc.sendBuffer {
			if err := wc.sendMessage(msg); err != nil {
				reconnectCount++
				if reconnectCount > wc.maxReconnectCount {
					wc.errorChan <- err
					return
				}

				time.Sleep(wc.reconnectDelay)
				if err := wc.reconnect(); err != nil {
					wc.errorChan <- err
					return
				}
				wc.onReconnect()
				break // 再接続後、バッファの先頭から再送するためループを抜ける
			} else {
				// 送信成功したメッセージはバッファから削除
				wc.sendBufferMutex.Lock()
				wc.sendBuffer = wc.sendBuffer[1:]
				wc.sendBufferMutex.Unlock()
			}
		}

		_, message, err := wc.conn.ReadMessage()
		if err != nil {
			reconnectCount++
			if reconnectCount > wc.maxReconnectCount {
				wc.errorChan <- err
				return
			}

			time.Sleep(wc.reconnectDelay)
			if err := wc.reconnect(); err != nil {
				wc.errorChan <- err
				return
			}
			wc.onReconnect()
			continue // 再接続後、メッセージの受信を再開するためループの先頭に戻る
		}
		wc.receiveChan <- string(message)
	}
}

func (wc *StreamingConnection) reconnect() error {
	wc.connMu.Lock()
	defer wc.connMu.Unlock()

	conn, _, err := websocket.DefaultDialer.Dial(wc.url, nil)
	if err != nil {
		return err
	}
	wc.conn = conn
	return nil
}

func (wc *StreamingConnection) sendMessage(msg interface{}) error {
	wc.connMu.Lock()
	defer wc.connMu.Unlock()

	bytes, err := json.Marshal(msg)
	if err != nil {
		return err
	}

	if err := wc.conn.WriteMessage(websocket.TextMessage, bytes); err != nil {
		return err
	}
	return nil
}
