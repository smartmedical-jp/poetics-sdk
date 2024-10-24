#ifndef TEMPORARY_UTTERANCE_MESSAGE_H
#define TEMPORARY_UTTERANCE_MESSAGE_H

#include <string>
using std::string;

#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace poetics::streaming::asr_job::incoming_message {
    class TemporaryUtteranceMessageBody {
    public:
        TemporaryUtteranceMessageBody();
        TemporaryUtteranceMessageBody(int _channelIndex, string _utteranceID, string _startAt, string _endAt, string _text);
        
        int channelIndex;
        string utteranceID;
        string startAt;
        string endAt;
        string text;
    };

    class TemporaryUtteranceMessage {
    public:
        TemporaryUtteranceMessage();
        TemporaryUtteranceMessage(string _message, TemporaryUtteranceMessageBody _body);

        string message;
        TemporaryUtteranceMessageBody body;
    };

    void from_json(const json& j, TemporaryUtteranceMessageBody& body);
    void from_json(const json& j, TemporaryUtteranceMessage& message);
}

#endif
