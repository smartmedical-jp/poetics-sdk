#ifndef CREATE_STREAM_ASR_JOB_MESSAGE_H
#define CREATE_STREAM_ASR_JOB_MESSAGE_H

#include <string>
using std::string;
#include <vector>

namespace poetics::streaming::asr_job::outgoing_message {
    class CreateStreamAsrJobMessageBodyChannel {
    public:
        CreateStreamAsrJobMessageBodyChannel(string participantName) : _participantName(participantName) {}

        string getParticipantName() { return _participantName; }
    private:
        string _participantName;
    };

    class CreateStreamAsrJobMessageBody {
    public:
        CreateStreamAsrJobMessageBody(bool enableDataLogging, string conversationTitle, string audioEncoding, 
            int audioSampleRate, int channelCount, std::vector<CreateStreamAsrJobMessageBodyChannel> channels) :
                _enableDataLogging(enableDataLogging), _conversationTitle(conversationTitle), _audioEncoding(audioEncoding),
                _audioSampleRate(audioSampleRate), _channelCount(channelCount), _channels(channels) {}
        
        bool getEnableDataLogging() { return _enableDataLogging; }
        string getConversationTitle() { return _conversationTitle; }
        string getAudioEncoding() { return _audioEncoding; }
        int getAudioSampleRate() { return _audioSampleRate; }
        int getChannelCount() { return _channelCount; }
        std::vector<CreateStreamAsrJobMessageBodyChannel> getChannels() { return _channels; }
    private:
        bool _enableDataLogging;
        string _conversationTitle;
        string _audioEncoding;
        int _audioSampleRate;
        int _channelCount;
        std::vector<CreateStreamAsrJobMessageBodyChannel> _channels;
    };

    class CreateStreamAsrJobMessage {
    public:
        CreateStreamAsrJobMessage(string command, CreateStreamAsrJobMessageBody body) : _command(command), _body(body) {}

        string getCommand() { return _command; }
        CreateStreamAsrJobMessageBody getBody() { return _body; }
    private:
        string _command;
        CreateStreamAsrJobMessageBody _body;
    };
}

#endif