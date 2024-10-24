#include "job_detail_message.h"

namespace poetics::streaming::asr_job::incoming_message {
    JobDetailMessageBody::JobDetailMessageBody() {}

    JobDetailMessageBody::JobDetailMessageBody(string streamAsrJobID, string conversationID, 
    bool isDataLoggingEnabled, string audioEncoding, int audioSampleRate, 
    string status, int channelCount, string createdAt) {
        this->streamAsrJobID = streamAsrJobID;
        this->conversationID = conversationID;
        this->isDataLoggingEnabled = isDataLoggingEnabled;
        this->audioEncoding = audioEncoding;
        this->audioSampleRate = audioSampleRate;
        this->status = status;
        this->channelCount = channelCount;
        this->createdAt = createdAt;
    }

    JobDetailMessage::JobDetailMessage() {}

    JobDetailMessage::JobDetailMessage(string message, JobDetailMessageBody body) : message(message), body(body) {}

    void from_json(const json& j, JobDetailMessageBody& body) {
        body.streamAsrJobID = j["stream_asr_job_id"].get<string>();
        body.conversationID = j["conversation_id"].get<string>();
        body.isDataLoggingEnabled = j["is_data_logging_enabled"].get<bool>();
        body.audioEncoding = j["audio_encoding"].get<string>();
        body.audioSampleRate = j["audio_sample_rate"].get<int>();
        body.status = j["status"].get<string>();
        body.channelCount = j["channel_count"].get<int>();
        body.createdAt = j["created_at"].get<string>();
    }

    void from_json(const json& j, JobDetailMessage& message) {
        message.message = j["message"].get<string>();
        JobDetailMessageBody body;
        from_json(j["Body"], body);
        message.body = body;
    }
}
