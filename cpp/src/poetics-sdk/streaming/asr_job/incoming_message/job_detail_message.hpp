#ifndef JOB_DETAIL_MESSAGE_H
#define JOB_DETAIL_MESSAGE_H

#include <string>
using std::string;

namespace poetics::streaming::asr_job::incoming_message {
    class JobDetailMessageBody {
    public:
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
    private:
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
        JobDetailMessage(string message, JobDetailMessageBody body) : _message(message), _body(body) {}

        string getJobDetail() { return _message; }
        JobDetailMessageBody getBody() { return _body; }
    private:
        string _message;
        JobDetailMessageBody _body;
    };
}

#endif