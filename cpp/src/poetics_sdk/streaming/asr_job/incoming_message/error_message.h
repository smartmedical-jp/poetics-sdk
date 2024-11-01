#ifndef ERROR_MESSAGE_H
#define ERROR_MESSAGE_H

#include <string>
using std::string;

#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace poetics::streaming::asr_job::incoming_message {
    class ErrorMessageBody{
    public:
        ErrorMessageBody();
        ErrorMessageBody(string _code, string _message);

        string code;
        string message;
    };

    class ErrorMessage {
    public:
        ErrorMessage();
        ErrorMessage(string _message, ErrorMessageBody _body);

        string message;
        ErrorMessageBody body;
    };

    void from_json(const json& j, ErrorMessage& e);
    void from_json(const json& j, ErrorMessageBody& b);
}

#endif
