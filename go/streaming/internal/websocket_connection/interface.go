package websocket_connection

type WebsocketConnectionInterface interface {
	SetOnReconnect(onReconnect func())

	Send(message interface{})
	Close()

	SubscribeMessage() chan string
	SubscribeError() chan error
	SubscribeDone() chan struct{}
}
