#ifndef ASR_JOB_CORE_CONNECTION_H
#define ASR_JOB_CORE_CONNECTION_H

#include <functional>

namespace poetics::streaming::asr_job::core {
    template <typename T>
    class IConnection {
    public:
        virtual ~IConnection() = default;
        void SetOnReconnect(std::function<void(void)> onReconnect) = 0;
        void Send(T message) = 0;
    };
}

#endif