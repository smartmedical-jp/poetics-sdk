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
        CreateStreamAsrJobMessageBodyChannel(string participantName);

        string participantName;
    };

    class CreateStreamAsrJobMessageBody {
    public:
        CreateStreamAsrJobMessageBody(bool enableDataLogging, string conversationTitle, string audioEncoding, 
            int audioSampleRate, int channelCount, std::vector<CreateStreamAsrJobMessageBodyChannel> channels);
        
        bool enableDataLogging;
        string conversationTitle;
        string audioEncoding;
        int audioSampleRate;
        int channelCount;
        std::vector<CreateStreamAsrJobMessageBodyChannel> channels;
    };

    class CreateStreamAsrJobMessage {
    public:
        CreateStreamAsrJobMessage(string command, CreateStreamAsrJobMessageBody body);

        string command;
        CreateStreamAsrJobMessageBody body;
    };

    void to_json(json& j, const CreateStreamAsrJobMessageBodyChannel& channel);
    
    void to_json(json& j, const CreateStreamAsrJobMessageBody& body);
    
    void to_json(json& j, const CreateStreamAsrJobMessage& message);
}

#endif
