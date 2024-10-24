#include "submit_audio_fragment_message.h"

namespace poetics::streaming::asr_job::outgoing_message {
    SubmitAudioFragmentMessageBody::SubmitAudioFragmentMessageBody()
    {
    }

    SubmitAudioFragmentMessageBody::SubmitAudioFragmentMessageBody(int _channelIndex, int _audioFragmentIndex, string _base64AudioData) 
        : channelIndex(_channelIndex), audioFragmentIndex(_audioFragmentIndex), base64AudioData(_base64AudioData)
    {
    }

    SubmitAudioFragmentMessage::SubmitAudioFragmentMessage(string _command, SubmitAudioFragmentMessageBody _body)
    {
        command = _command;
        body = _body;
    }

    void to_json(json &j, const SubmitAudioFragmentMessageBody &body)
    {
        j["channel_index"] = body.channelIndex;
        j["audio_fragment_index"] = body.audioFragmentIndex;
        j["base64_audio_data"] = body.base64AudioData;
    }

    void to_json(json &j, const SubmitAudioFragmentMessage &message)
    {
        j = json{
            {"command", message.command},
            {"body", message.body}
        };
    }
}
