#include "temporary_utterance_message.h"


namespace poetics::streaming::asr_job::incoming_message {
    TemporaryUtteranceMessageBody::TemporaryUtteranceMessageBody() 
        : channelIndex(0), utteranceID(""), startAt(""), endAt(""), text("") {}
    TemporaryUtteranceMessageBody::TemporaryUtteranceMessageBody(
        int _channelIndex, string _utteranceID, string _startAt, string _endAt, string _text)
        : channelIndex(_channelIndex), utteranceID(_utteranceID), startAt(_startAt), endAt(_endAt), text(_text) {}

    TemporaryUtteranceMessage::TemporaryUtteranceMessage()
        : message(""), body(TemporaryUtteranceMessageBody()) {}
    TemporaryUtteranceMessage::TemporaryUtteranceMessage(string _message, TemporaryUtteranceMessageBody _body)
        : message(_message), body(_body) {}

    void from_json(const json& j, TemporaryUtteranceMessageBody& body) {
        body.channelIndex = j["channel_index"];
        body.utteranceID = j["utterance_id"];
        body.startAt = j["start_at"];
        body.endAt = j["end_at"];
        body.text = j["text"];
    }

    void from_json(const json& j, TemporaryUtteranceMessage& message) {
        message.message = j["message"];
        from_json(j["Body"], message.body);
    }
}
