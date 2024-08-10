#ifndef JOB_STATUS_UPDATED_MESSAGE_H
#define JOB_STATUS_UPDATED_MESSAGE_H

#include <string>
using std::string;

namespace poetics::streaming::asr_job::incoming_message {
    class JobStatusUpdatedMessageBody {
    public:
        JobStatusUpdatedMessageBody(string status) : _status(status) {}

        string getStatus() { return _status; }
    private:
        string _status;
    };

    class JobStatusUpdatedMessage {
    public:
        JobStatusUpdatedMessage(string message, JobStatusUpdatedMessageBody body) : _message(message), _body(body) {}

        string getMessage() { return _message; }
        JobStatusUpdatedMessageBody getBody() { return _body; }
    private:
        string _message;
        JobStatusUpdatedMessageBody _body;
    };
}

#endif