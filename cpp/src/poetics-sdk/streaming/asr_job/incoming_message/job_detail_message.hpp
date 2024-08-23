#ifndef JOB_DETAIL_MESSAGE_H
#define JOB_DETAIL_MESSAGE_H

#include <string>
using std::string;

namespace poetics::streaming::asr_job::incoming_message {
    class JobDetailMessageBody {
    public:
        JobDetailMessageBody() {}
        JobDetailMessageBody(string streamAsrJobID, string conversationID, 
        bool isDataLoggingEnabled, string audioEncoding, int audioSampleRate, 
        string status, int channelCount, string createdAt){
            _streamAsrJobID = streamAsrJobID;
            _conversationID = conversationID;
            _isDataLoggingEnabled = isDataLoggingEnabled;
            _audioEncoding = audioEncoding;
            _audioSampleRate = audioSampleRate;
            _status = status;
            _channelCount = channelCount;
            _createdAt = createdAt;
        }

        string getStreamAsrJobID() { return _streamAsrJobID; }
        string getConversationID() { return _conversationID; }
        bool getIsDataLoggingEnabled() { return _isDataLoggingEnabled; }
        string getAudioEncoding() { return _audioEncoding; }
        int getAudioSampleRate() { return _audioSampleRate; }
        string getStatus() { return _status; }
        int getChannelCount() { return _channelCount; }
        string getCreatedAt() { return _createdAt; }

        string _streamAsrJobID;
        string _conversationID;
        bool _isDataLoggingEnabled;
        string _audioEncoding;
        int _audioSampleRate;
        string _status;
        int _channelCount;
        string _createdAt; // Should be replaced with <chrono>?
    };

    class JobDetailMessage {
    public:
        JobDetailMessage() {}
        JobDetailMessage(string message, JobDetailMessageBody body) : _message(message), _body(body) {}

        string getJobDetail() { return _message; }
        JobDetailMessageBody getBody() { return _body; }

        string _message;
        JobDetailMessageBody _body;
    };

    void from_json(const json& j, JobDetailMessageBody& body) {
        body._streamAsrJobID = j["stream_asr_job_id"].get<string>();
        body._conversationID = j["conversation_id"].get<string>();
        body._isDataLoggingEnabled = j["is_data_logging_enabled"].get<bool>();
        body._audioEncoding = j["audio_encoding"].get<string>();
        body._audioSampleRate = j["audio_sample_rate"].get<int>();
        body._status = j["status"].get<string>();
        body._channelCount = j["channel_count"].get<int>();
        body._createdAt = j["created_at"].get<string>();
    }

    void from_json(const json& j, JobDetailMessage& message) {
        message._message = j["message"].get<string>();
        JobDetailMessageBody body;
        from_json(j["body"], body);
        message._body = body;
    }
}

#endif