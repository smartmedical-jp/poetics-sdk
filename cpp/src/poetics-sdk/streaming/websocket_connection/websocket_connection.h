#ifndef WEBSOCKET_CONNECTION_H
#define WEBSOCKET_CONNECTION_H

#include <functional>
#include <string>
using std::string;
#include <mutex>

#include "../../poetics_error.h"

namespace poetics::streaming::websocket_connection {
    template <typename T>
    class IWebsocketConnection
    {
    public:
        virtual ~IWebsocketConnection();
        virtual void SetOnReconnect(std::function<void(void)> onReconnect) = 0;
        
        virtual void Send(T message) = 0;
        virtual void Close() = 0;

        virtual string SubscribeMessaage() = 0; // Should be chan string
        virtual poetics::Error SubscribeError() = 0; // Should be chan Error
        virtual T SubscribeDone() = 0;  // Is chan struct{} in golang SDK
    };

    template <typename T>
    class WebsocketConnection : public IWebsocketConnection<T>
    {
    public:
        ~WebsocketConnection();
        void SetOnReconnect(std::function<void(void)> onReconnect) override;
        void Send(T message) override;
        void Close() override;
        string SubscribeMessaage() override;
        poetics::Error SubscribeError() override;
        T SubscribeDone() override;

    private:
        string url;
        // Conn* conn
        string receiveChan; // chan
        poetics::Error errorChan; // chan
        T doneChan; // chan
        std::vector<std::string> messageBuffer;
        std::mutex mutex;
        int remainingReconnectAttempts;
        std::function<void(void)> onReconnect;
        bool isClosed;
        poetics::Error lastReconnectError;
    };
}

#endif
