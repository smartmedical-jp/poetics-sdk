#include "websocket_connection.h"

namespace poetics::streaming::websocket_connection {
    template <typename T>
    inline WebsocketConnection<T>::~WebsocketConnection()
    {
    }

    template <typename T>
    void WebsocketConnection<T>::SetOnReconnect(std::function<void(void)> onReconnect)
    {
    }

    template <typename T>
    void WebsocketConnection<T>::Send(T message)
    {
    }

    template <typename T>
    void WebsocketConnection<T>::Close()
    {
    }

    template <typename T>
    string WebsocketConnection<T>::SubscribeMessaage()
    {
        return string();
    }

    template <typename T>
    poetics::Error WebsocketConnection<T>::SubscribeError()
    {
        return poetics::Error();
    }

    template <typename T>
    T WebsocketConnection<T>::SubscribeDone()
    {
        return T();
    }
}
