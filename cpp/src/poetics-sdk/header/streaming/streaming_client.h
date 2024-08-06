#ifndef STREAMING_CLIENT_H
#define STREAMING_CLIENT_H

#include <string>
using std::string;

#include <fmt/core.h>

#include "asr_job/core/options.h"
#include "asr_job/stream_asr_job.h"
using poetics::streaming::asr_job::StreamAsrJob;
using namespace poetics::streaming::asr_job::core;

namespace poetics::streaming {
    class StreamingClientOptions;

    class StreamingClient
    {
    private:
        string url;
    public:
        StreamingClient(StreamingClientOptions options);

        StreamAsrJob createStreamAsrJob(CreateStreamAsrJobOptions options);
        StreamAsrJob connectToStreamAsrJob(ConnectToStreamAsrJobOptions options);
    };

    class StreamingClientOptions
    {
    private:
        string _apiKey;
        string _endPoint;
    public:
        StreamingClientOptions(string apiKey, string endPoint)
        {
            _apiKey = apiKey;
            _endPoint = endPoint;
        }

        string getApiKey() { return _apiKey; }
        string getEndPoint() { return _endPoint; }
    };
}

#endif