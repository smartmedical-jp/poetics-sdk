#include "job_status_updated_message.h"

namespace poetics::streaming::asr_job::incoming_message {
    JobStatusUpdatedMessage::JobStatusUpdatedMessage() {}
    JobStatusUpdatedMessage::JobStatusUpdatedMessage(string _message, JobStatusUpdatedMessageBody _body) : message(_message), body(_body) {}
    JobStatusUpdatedMessageBody::JobStatusUpdatedMessageBody() {}
    JobStatusUpdatedMessageBody::JobStatusUpdatedMessageBody(string _status) : status(_status) {}

    void from_json(const json& j, JobStatusUpdatedMessageBody& body) {
        j.at("status").get_to(body.status);
    }

    void from_json(const json& j, JobStatusUpdatedMessage& message) {
        j.at("message").get_to(message.message);
        j.at("Body").get_to(message.body);
    }
}
