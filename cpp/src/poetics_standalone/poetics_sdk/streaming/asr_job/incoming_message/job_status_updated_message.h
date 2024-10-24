#ifndef JOB_STATUS_UPDATED_MESSAGE_H
#define JOB_STATUS_UPDATED_MESSAGE_H

#include <string>
using std::string;

#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace poetics::streaming::asr_job::incoming_message {
    class JobStatusUpdatedMessageBody {
    public:
        JobStatusUpdatedMessageBody();
        JobStatusUpdatedMessageBody(string _status);

        string status;
    };

    class JobStatusUpdatedMessage {
    public:
        JobStatusUpdatedMessage();
        JobStatusUpdatedMessage(string _message, JobStatusUpdatedMessageBody _body);

        string message;
        JobStatusUpdatedMessageBody body;
    };

    void from_json(const json& j, JobStatusUpdatedMessageBody& body);
    void from_json(const json& j, JobStatusUpdatedMessage& message);
}

#endif
