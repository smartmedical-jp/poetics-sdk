#include "error_message.h"

namespace poetics::streaming::asr_job::incoming_message {
    ErrorMessageBody::ErrorMessageBody() : code(""), message("") {}
    ErrorMessageBody::ErrorMessageBody(string code, string message) : code(code), message(message) {}

    ErrorMessage::ErrorMessage() {}
    ErrorMessage::ErrorMessage(string message, ErrorMessageBody body) : message(message), body(body) {}

    void from_json(const json& j, ErrorMessage& e) {
        j.at("message").get_to(e.message);
        j.at("body").get_to(e.body);
    }

    void from_json(const json& j, ErrorMessageBody& b) {
        j.at("code").get_to(b.code);
        j.at("message").get_to(b.message);
    }
}
