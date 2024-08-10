#ifndef ERROR_MESSAGE_H
#define ERROR_MESSAGE_H

#include <string>
using std::string;

namespace poetics::streaming::asr_job::incoming_message {
    class ErrorMessageBody{
    public:
        ErrorMessageBody(string code, string message) : _code(code), _message(message) {}

        string getCode() { return _code; }
        string getMessage() { return _message; }
    private:
        string _code;
        string _message;
    };

    class ErrorMessage {
    public:
        ErrorMessage(string message, ErrorMessageBody body) : _message(message), _body(body) {}

        string getMessage() { return _message; }
        ErrorMessageBody getBody() { return _body; }
    private:
        string _message;
        ErrorMessageBody _body;
    };
}

#endif