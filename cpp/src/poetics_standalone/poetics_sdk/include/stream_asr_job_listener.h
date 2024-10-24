#ifndef STREAM_ASR_JOB_LISTENER_H
#define STREAM_ASR_JOB_LISTENER_H

#include <string>
using std::string;

namespace poetics::streaming::asr_job {
    class StreamAsrJobListener {
    public: 
        virtual void OnMessageReceived(const string result) = 0;

        virtual void OnErrorReceived(const string error) = 0;

        virtual void OnDebugMessageReceived(const string message) = 0;
    };
}

#endif