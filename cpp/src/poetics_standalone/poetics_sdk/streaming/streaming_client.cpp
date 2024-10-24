#include "../include/streaming_client.h"
#include "asr_job/core/stream_asr_job_core.h"
using namespace poetics::streaming::asr_job::core;

namespace poetics::streaming {
    StreamingClient::StreamingClient(string endpoint, string apiKey)
    {
        _endpoint = endpoint;
        _apiKey = apiKey;
    }

    std::unique_ptr<StreamAsrJob> StreamingClient::createStreamAsrJob(
        string audioEncoding, int audioSampleRate, int channelCount, 
        bool enableDatalogging, string conversationTitle, vector<string>& channels)
    {
        auto asrJob = std::make_unique<StreamAsrJobCore>(
            _endpoint, _apiKey, audioEncoding, audioSampleRate, 
            channelCount, enableDatalogging, conversationTitle, channels);
        return asrJob;
    }

    // Not implemented yet
    std::unique_ptr<StreamAsrJob> poetics::streaming::StreamingClient::connectToStreamAsrJob(string streamAsrJobID, int channelCount)
    {
        auto asrJob = std::make_unique<StreamAsrJobCore>(
            _endpoint, _apiKey, streamAsrJobID, channelCount);
        return asrJob;
    }
}


