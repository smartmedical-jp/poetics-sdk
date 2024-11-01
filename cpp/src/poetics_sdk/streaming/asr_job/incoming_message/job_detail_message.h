#ifndef JOB_DETAIL_MESSAGE_H
#define JOB_DETAIL_MESSAGE_H

#include <string>
using std::string;

#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace poetics::streaming::asr_job::incoming_message {
    class JobDetailMessageBody {
    public:
        JobDetailMessageBody();
        JobDetailMessageBody(string streamAsrJobID, string conversationID, 
        bool isDataLoggingEnabled, string audioEncoding, int audioSampleRate, 
        string status, int channelCount, string createdAt);

        string streamAsrJobID;
        string conversationID;
        bool isDataLoggingEnabled;
        string audioEncoding;
        int audioSampleRate;
        string status;
        int channelCount;
        string createdAt; // Should be replaced with <chrono>?
    };

    class JobDetailMessage {
    public:
        JobDetailMessage();
        JobDetailMessage(string message, JobDetailMessageBody body);

        string message;
        JobDetailMessageBody body;
    };

    void from_json(const json& j, JobDetailMessageBody& body);
    void from_json(const json& j, JobDetailMessage& message);
}

#endif
