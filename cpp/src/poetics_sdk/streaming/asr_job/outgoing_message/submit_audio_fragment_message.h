#ifndef SUBMIT_AUDIO_FRAGMENT_MESSAGE_H
#define SUBMIT_AUDIO_FRAGMENT_MESSAGE_H

#include <string>
using std::string;

#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace poetics::streaming::asr_job::outgoing_message {
    class SubmitAudioFragmentMessageBody {
    public:
        SubmitAudioFragmentMessageBody();
        SubmitAudioFragmentMessageBody(int _channelIndex, int _audioFragmentIndex, string _base64AudioData);

        int channelIndex;
        int audioFragmentIndex;
        string base64AudioData;
    };

    class SubmitAudioFragmentMessage {
    public:
        SubmitAudioFragmentMessage(string _command, SubmitAudioFragmentMessageBody _body);

        string command;
        SubmitAudioFragmentMessageBody body;
    };

    void to_json(json& j, const SubmitAudioFragmentMessageBody& body);
    void to_json(json& j, const SubmitAudioFragmentMessage& message);
}

#endif
