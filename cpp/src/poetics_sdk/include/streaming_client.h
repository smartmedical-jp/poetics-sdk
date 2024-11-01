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

// Not declaring namespace like poetics::streaming since it's supported from C++20
// This is to make it compatible with C++14
namespace poetics {
    namespace streaming {
        class StreamingClient
        {
        public:
            POETICS_API StreamingClient(const char * endpoint, const char * apiKey);

            // Used when you want to create and connect to a new streamAsrJob
            POETICS_API std::unique_ptr<StreamAsrJob> createStreamAsrJob(const char * audioEncoding, int audioSampleRate, 
                int channelCount, bool enableDatalogging, const char * conversationTitle, vector<const char *>& channels);

            // Used when you already have a streamAsrJobID and want to connect to it
            POETICS_API std::unique_ptr<StreamAsrJob> connectToStreamAsrJob(const char * streamAsrJobID, int channelCount);
        private: 
            unique_ptr<string> _endpoint;
            unique_ptr<string> _apiKey;
        };
    }
}

#endif