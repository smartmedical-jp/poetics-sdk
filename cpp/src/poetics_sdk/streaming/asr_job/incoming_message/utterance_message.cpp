#include "utterance_message.h"

namespace poetics::streaming::asr_job::incoming_message {
    UtteranceMessageBody::UtteranceMessageBody() : channelIndex(0), utteranceID(""), startAt(""), endAt(""), text("") {}
    UtteranceMessageBody::UtteranceMessageBody(int _channelIndex, string _utteranceID, string _startAt, string _endAt, string _text) : 
        channelIndex(channelIndex), utteranceID(utteranceID), startAt(startAt), endAt(endAt), text(_text) {}

    UtteranceMessage::UtteranceMessage() : message(""), body(UtteranceMessageBody()) {}
    UtteranceMessage::UtteranceMessage(string message, UtteranceMessageBody body) : message(message), body(body) {}

    void from_json(const json& j, UtteranceMessageBody& body) {
        j.at("channel_index").get_to(body.channelIndex);
        j.at("utterance_id").get_to(body.utteranceID);
        j.at("start_at").get_to(body.startAt);
        j.at("end_at").get_to(body.endAt);
        j.at("text").get_to(body.text);
    }

    void from_json(const json& j, UtteranceMessage& message) {
        j.at("message").get_to(message.message);
        j.at("body").get_to(message.body);
    }
}
