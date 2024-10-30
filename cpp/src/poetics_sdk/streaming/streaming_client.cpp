#include "../include/streaming_client.h"
#include "asr_job/core/stream_asr_job_core.h"
using namespace poetics::streaming::asr_job::core;

namespace poetics::streaming {
    StreamingClient::StreamingClient(const char * endpoint, const char * apiKey)
    {
        _endpoint = std::make_unique<string>(endpoint);
        _apiKey = std::make_unique<string>(apiKey);
    }

    std::unique_ptr<StreamAsrJob> StreamingClient::createStreamAsrJob(
        const char * audioEncoding, int audioSampleRate, int channelCount, 
        bool enableDatalogging, const char * conversationTitle, vector<const char *>& channels)
    {
        auto asrJob = std::make_unique<StreamAsrJobCore>(
            _endpoint->c_str(), _apiKey->c_str(), audioEncoding, audioSampleRate, 
            channelCount, enableDatalogging, conversationTitle, channels);
        return asrJob;
    }

    // Not implemented yet
    std::unique_ptr<StreamAsrJob> poetics::streaming::StreamingClient::connectToStreamAsrJob(const char * streamAsrJobID, int channelCount)
    {
        auto asrJob = std::make_unique<StreamAsrJobCore>(
            _endpoint->c_str(), _apiKey->c_str(), streamAsrJobID, channelCount);
        return asrJob;
    }
}


