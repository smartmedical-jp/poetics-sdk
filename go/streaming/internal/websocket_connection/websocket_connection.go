package websocket_connection

import (
	"encoding/json"
	"sync"
	"time"

	"github.com/gorilla/websocket"
	"github.com/smartmedical-jp/poetics-sdk/go/streaming/internal/logging"
)

type WebsocketConnection struct {
	url                        string
	conn                       *websocket.Conn
	receiveChan                chan string
	errorChan                  chan error
	doneChan                   chan struct{}
	messageBuffer              []string
	mutex                      sync.Mutex
	remainingReconnectAttempts int
	onReconnect                func()
	isClosed                   bool
	lastReconnectError         error
}

var _ WebsocketConnectionInterface = &WebsocketConnection{}

func NewWebsocketConnection(url string) (*WebsocketConnection, error) {

	c := &WebsocketConnection{
		url:                        url,
		conn:                       nil,
		receiveChan:                make(chan string, 100),
		errorChan:                  make(chan error, 1),
		doneChan:                   make(chan struct{}, 1),
		messageBuffer:              make([]string, 0),
		remainingReconnectAttempts: 10,
		onReconnect:                func() {},
		isClosed:                   false,
	}

	if err := c.connect(); err != nil {
		return nil, err
	}

	go c.receiveLoop()
	go c.sendLoop()
	go c.pingLoop()

	return c, nil
}

func (c *WebsocketConnection) connect() error {
	logging.Logger.Debug("WebsocketConnection.connect()")
	var err error
	c.conn, _, err = websocket.DefaultDialer.Dial(c.url, nil)
	return err
}

func (c *WebsocketConnection) SetOnReconnect(onReconnect func()) {
	c.onReconnect = onReconnect
}

func (c *WebsocketConnection) Send(message interface{}) {
	messageStr, err := json.Marshal(message)
	if err != nil {
		return
	}
	c.mutex.Lock()
	defer c.mutex.Unlock()
	c.messageBuffer = append(c.messageBuffer, string(messageStr))
}

func (c *WebsocketConnection) Close() {
	logging.Logger.Debug("WebsocketConnection.Close()")
	c.mutex.Lock()
	if !c.isClosed {
		c.isClosed = true
		c.conn.Close()
		close(c.receiveChan)
		close(c.errorChan)
		close(c.doneChan)
	}
	c.mutex.Unlock()
}

func (c *WebsocketConnection) SubscribeMessage() chan string {
	return c.receiveChan
}

func (c *WebsocketConnection) SubscribeError() chan error {
	return c.errorChan
}

func (c *WebsocketConnection) SubscribeDone() chan struct{} {
	return c.doneChan
}

func (c *WebsocketConnection) pingLoop() {
	ticker := time.NewTicker(3 * time.Second)
	defer ticker.Stop()

	for range ticker.C {
		if c.isClosed {
			return
		}

		logging.Logger.Debug("PING")
		if err := c.conn.WriteControl(websocket.PingMessage, []byte{}, time.Now().Add(5*time.Second)); err != nil {
			logging.Logger.Debug("PING", "error", err.Error())
			c.reconnect()
		}
	}
}

func (c *WebsocketConnection) receiveLoop() {
	for {

		if c.isClosed {
			return
		}

		_, message, err := c.conn.ReadMessage()
		if err != nil {
			time.Sleep(500 * time.Millisecond)
			continue
		}

		if !c.isClosed {
			logging.Logger.Debug("RCV", "message", string(message))
			c.receiveChan <- string(message)
		}
	}
}

func (c *WebsocketConnection) sendLoop() {
	for {
		if c.isClosed {
			return
		}

		if len(c.messageBuffer) == 0 {
			continue
		}

		message := c.messageBuffer[0]

		logging.Logger.Debug("SND", "message", logging.TrimString(message, 100))
		if err := c.conn.WriteMessage(websocket.TextMessage, []byte(message)); err != nil {
			time.Sleep(500 * time.Millisecond)
			continue
		}

		c.mutex.Lock()
		c.messageBuffer = c.messageBuffer[1:]
		c.mutex.Unlock()
	}
}

func (c *WebsocketConnection) reconnect() {
	logging.Logger.Debug("WebsocketConnection.reconnect()")

	c.remainingReconnectAttempts--
	if c.remainingReconnectAttempts < 0 {
		logging.Logger.Warn("max reconnect attempts reached")
		c.errorChan <- c.lastReconnectError
		c.Close()
		return
	}

	if err := c.connect(); err != nil {
		logging.Logger.Warn("failed to reconnect", "error", err.Error())
		c.lastReconnectError = err
		time.Sleep(1 * time.Second)
		c.reconnect()
		return
	}

	if c.onReconnect != nil {
		c.onReconnect()
	}
}
