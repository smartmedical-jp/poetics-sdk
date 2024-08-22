#ifndef CREATE_STREAM_ASR_JOB_MESSAGE_H
#define CREATE_STREAM_ASR_JOB_MESSAGE_H

#include <string>
using std::string;
#include <vector>

#include <nlohmann/json.hpp>
using nlohmann::json;

namespace poetics::streaming::asr_job::outgoing_message {
    class CreateStreamAsrJobMessageBodyChannel {
    public:
        CreateStreamAsrJobMessageBodyChannel(string participantName) : _participantName(participantName) {}

        string getParticipantName() const { return _participantName; }

        string _participantName;
    };

    class CreateStreamAsrJobMessageBody {
    public:
        CreateStreamAsrJobMessageBody(bool enableDataLogging, string conversationTitle, string audioEncoding, 
            int audioSampleRate, int channelCount, std::vector<CreateStreamAsrJobMessageBodyChannel> channels) :
                _enableDataLogging(enableDataLogging), _conversationTitle(conversationTitle), _audioEncoding(audioEncoding),
                _audioSampleRate(audioSampleRate), _channelCount(channelCount), _channels(channels) {}
        
        bool getEnableDataLogging() const { return _enableDataLogging; }
        string getConversationTitle() const { return _conversationTitle; }
        string getAudioEncoding() const { return _audioEncoding; }
        int getAudioSampleRate() const { return _audioSampleRate; }
        int getChannelCount() const { return _channelCount; }
        std::vector<CreateStreamAsrJobMessageBodyChannel> getChannels() const { return _channels; }

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

        string getCommand() const { return _command; }
        CreateStreamAsrJobMessageBody getBody() const { return _body; }

        string _command;
        CreateStreamAsrJobMessageBody _body;
    };

    void to_json(json& j, const CreateStreamAsrJobMessageBodyChannel& channel) {
        j = json{
            {"participant_name", channel.getParticipantName()}
        };
    }
    
    void to_json(json& j, const CreateStreamAsrJobMessageBody& body) {
        j = json{
            {"enable_data_logging", body.getEnableDataLogging()},
            {"conversation_title", body.getConversationTitle()},
            {"audio_encoding", body.getAudioEncoding()},
            {"audio_sample_rate", body.getAudioSampleRate()},
            {"channel_count", body.getChannelCount()},
            {"channels", body.getChannels()}
        };
    }
    
    void to_json(json& j, const CreateStreamAsrJobMessage& message) {
        j = json{
            {"command", message.getCommand()},
            {"body", message.getBody()}
        };
    }
}

#endif