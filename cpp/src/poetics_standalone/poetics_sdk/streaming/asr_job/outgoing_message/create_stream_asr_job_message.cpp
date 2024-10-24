#include "create_stream_asr_job_message.h"

namespace poetics::streaming::asr_job::outgoing_message {
    CreateStreamAsrJobMessageBodyChannel::CreateStreamAsrJobMessageBodyChannel(string participantName) 
        : participantName(participantName) {}

    CreateStreamAsrJobMessageBody::CreateStreamAsrJobMessageBody(bool enableDataLogging, string conversationTitle, string audioEncoding, 
        int audioSampleRate, int channelCount, std::vector<CreateStreamAsrJobMessageBodyChannel> channels) 
        : enableDataLogging(enableDataLogging), conversationTitle(conversationTitle), audioEncoding(audioEncoding),
        audioSampleRate(audioSampleRate), channelCount(channelCount), channels(channels) {}

    CreateStreamAsrJobMessage::CreateStreamAsrJobMessage(string command, CreateStreamAsrJobMessageBody body) 
        : command(command), body(body) {}

    void to_json(json& j, const CreateStreamAsrJobMessageBodyChannel& channel) {
        j = json{
            {"participant_name", channel.participantName}
        };
    }
    
    void to_json(json& j, const CreateStreamAsrJobMessageBody& body) {
        j = json{
            {"enable_data_logging", body.enableDataLogging},
            {"conversation_title", body.conversationTitle},
            {"audio_encoding", body.audioEncoding},
            {"audio_sample_rate", body.audioSampleRate},
            {"channel_count", body.channelCount},
            {"channels", body.channels}
        };
    }
    
    void to_json(json& j, const CreateStreamAsrJobMessage& message) {
        j = json{
            {"command", message.command},
            {"body", message.body}
        };
    }
}
