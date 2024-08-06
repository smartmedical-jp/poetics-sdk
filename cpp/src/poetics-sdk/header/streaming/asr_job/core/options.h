#ifndef ASR_JOB_OPTIONS_H
#define ASR_JOB_OPTIONS_H

#include <string>
using std::string;
#include <vector>
using std::vector;

namespace poetics::streaming::asr_job::core {
    class CreateStreamAsrJobOptionsChannel;

    class CreateStreamAsrJobOptions
    {
    private:
        string _audioEncoding;  // validate:"required,oneof=s16le mulaw"
        int _audioSampleRate;   // validate:"required,oneof=8000 11000 16000 22000 32000 44100 48000"
        int _channelCount;      // validate:"required,oneof=1 2"

        bool _enableDataLogging;
        string _conversationTitle;
        vector<CreateStreamAsrJobOptionsChannel> _channels;
    public:
        CreateStreamAsrJobOptions(string audioEncoding, int audioSampleRate, int channelCount,
            bool enableDatalogging = false, string conversationTitle = "", vector<CreateStreamAsrJobOptionsChannel> channels = {})
        {
            _audioEncoding = audioEncoding;
            _audioSampleRate = audioSampleRate;
            _channelCount = channelCount;
            _enableDataLogging = enableDatalogging;
            _conversationTitle = conversationTitle;
            _channels = channels;
        }

        string getAudioEncoding() { return _audioEncoding; }
        int getAudioSampleRate() { return _audioSampleRate; }
        int getChannelCount() { return _channelCount; }
        bool getEnableDataLogging() { return _enableDataLogging; }
        string getConversationTitle() { return _conversationTitle; }
        vector<CreateStreamAsrJobOptionsChannel> getChannels() { return _channels; }
    };

    class CreateStreamAsrJobOptionsChannel
    {
    private:
        string _participantName;
    public:
        CreateStreamAsrJobOptionsChannel(string participantName)
        {
            _participantName = participantName;
        }

        string getParticipantName() { return _participantName; }
    };

    class ConnectToStreamAsrJobOptions
    {
    private:
        string _streamAsrJobID;
        int _channelCount;
    public:
        ConnectToStreamAsrJobOptions(string streamAsrJobID, int channelCount)
        {
            _streamAsrJobID = streamAsrJobID;
            _channelCount = channelCount;
        }

        string getStreamAsrJobID() { return _streamAsrJobID; }
        int getChannelCount() { return _channelCount; }
    };
}

#endif