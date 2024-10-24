#ifndef UTTERANCE_MESSAGE_H
#define UTTERANCE_MESSAGE_H

#include <string>
using std::string;

#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace poetics::streaming::asr_job::incoming_message {
    class UtteranceMessageBody {
    public:
        UtteranceMessageBody();
        UtteranceMessageBody(int _channelIndex, string _utteranceID, string _startAt, string _endAt, string _text);

        int channelIndex;
        string utteranceID;
        string startAt; // Should be replaced to <chrono>?
        string endAt; // Should be replaced to <chrono>?
        string text;
    };

    class UtteranceMessage {
    public:
        UtteranceMessage();
        UtteranceMessage(string message, UtteranceMessageBody body);

        string message;
        UtteranceMessageBody body;
    };

    void from_json(const json& j, UtteranceMessageBody& body);
    void from_json(const json& j, UtteranceMessage& message);
}

#endif
