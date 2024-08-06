#ifndef STREAM_ASR_JOB_DETAIL_H
#define STREAM_ASR_JOB_DETAIL_H

#include <string>
using std::string;

class StreamAsrJobDetail
{
public:
    StreamAsrJobDetail(string streamAsrJobID, string conversationID,
                       bool isDataLoggingEnabled, string audioEncoding, int audioSampleRate,
                       string status, int channelCount, string createdAt)
    {
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
    [[deprecated("Will be changed to <chrono>")]]
    string getCreatedAt() { return _createdAt; }
private:
    string _streamAsrJobID;
    string _conversationID;
    bool _isDataLoggingEnabled;
    string _audioEncoding;
    int _audioSampleRate;
    string _status;
    int _channelCount;
    string _createdAt; // TODO: Should I change to <chrono>?
};

#endif