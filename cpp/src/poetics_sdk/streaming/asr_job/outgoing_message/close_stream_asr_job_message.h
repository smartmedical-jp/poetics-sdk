#ifndef CLOSE_STREAM_ASR_JOB_MESSAGE_H
#define CLOSE_STREAM_ASR_JOB_MESSAGE_H

#include <string>
using std::string;
#include <vector>

#include <nlohmann/json.hpp>
using nlohmann::json;

namespace poetics::streaming::asr_job::outgoing_message {
    class CloseStreamAsrJobMessage {
    public:
        CloseStreamAsrJobMessage(string _command);
    
        static CloseStreamAsrJobMessage CreateDefault();

        string command;
    };

    void to_json(json& j, const CloseStreamAsrJobMessage& message);
}

#endif
