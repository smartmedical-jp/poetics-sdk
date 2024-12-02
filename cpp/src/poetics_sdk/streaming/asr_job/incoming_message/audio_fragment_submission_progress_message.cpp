#include "audio_fragment_submission_progress_message.h"

namespace poetics::streaming::asr_job::incoming_message {
    AudioFragmentSubmissionProgressMessageBodyChannel::AudioFragmentSubmissionProgressMessageBodyChannel() : channelIndex(0), audioFragmentCount(0) {}
    AudioFragmentSubmissionProgressMessageBodyChannel::AudioFragmentSubmissionProgressMessageBodyChannel
        (int channelIndex, int audioFragmentCount) : channelIndex(channelIndex), audioFragmentCount(audioFragmentCount) {}

    AudioFragmentSubmissionProgressMessageBody::AudioFragmentSubmissionProgressMessageBody() {}
    AudioFragmentSubmissionProgressMessageBody::AudioFragmentSubmissionProgressMessageBody
        (std::vector<AudioFragmentSubmissionProgressMessageBodyChannel> channels) : channels(channels) {}

    AudioFragmentSubmissionProgressMessage::AudioFragmentSubmissionProgressMessage() : message(""), body() {}
    AudioFragmentSubmissionProgressMessage::AudioFragmentSubmissionProgressMessage
        (string message, AudioFragmentSubmissionProgressMessageBody body) : message(message), body(body) {}

    
    void from_json(const json& j, AudioFragmentSubmissionProgressMessageBody& body) {
        for (auto& channel : j["channels"]) {
            AudioFragmentSubmissionProgressMessageBodyChannel channelBody
                (channel["channel_index"], channel["audio_fragment_count"]);
            body.channels.push_back(channelBody);
        }
    }

    void from_json(const json& j, AudioFragmentSubmissionProgressMessage& message) {
        message.message = j["message"].get<string>();
        from_json(j["body"], message.body);
    }

    void from_json(const json& j, AudioFragmentSubmissionProgressMessageBodyChannel& channel) {
        channel.channelIndex = j["channel_index"].get<int>();
        channel.audioFragmentCount = j["audio_fragment_count"].get<int>();
    }
}
