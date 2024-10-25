#ifndef STREAMING_CLIENT_H
#define STREAMING_CLIENT_H

#include "api_definition.h"

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <memory>
using std::unique_ptr;

#include "stream_asr_job.h"
using poetics::streaming::asr_job::StreamAsrJob;


namespace poetics::streaming {
    class POETICS_API StreamingClient
    {
    public:
        StreamingClient(string endpoint, string apiKey);

        // Used when you want to create and connect to a new streamAsrJob
        std::unique_ptr<StreamAsrJob> createStreamAsrJob(string audioEncoding, int audioSampleRate, 
            int channelCount, bool enableDatalogging, string conversationTitle, vector<string>& channels);

        // Used when you already have a streamAsrJobID and want to connect to it
        std::unique_ptr<StreamAsrJob> connectToStreamAsrJob(string streamAsrJobID, int channelCount);
    private: 
        string _endpoint;
        string _apiKey;
    };
}

#endif