#ifndef AUDIO_FRAGMENT_SUBMISSION_PROGRESS_MESSAGE_H
#define AUDIO_FRAGMENT_SUBMISSION_PROGRESS_MESSAGE_H

#include <string>
using std::string;
#include <vector>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace poetics::streaming::asr_job::incoming_message {
    class AudioFragmentSubmissionProgressMessageBodyChannel {
    public:
        AudioFragmentSubmissionProgressMessageBodyChannel();
        AudioFragmentSubmissionProgressMessageBodyChannel(int channelIndex, int audioFragmentCount);

        int channelIndex;
        int audioFragmentCount;
    };

    class AudioFragmentSubmissionProgressMessageBody {
    public:
        AudioFragmentSubmissionProgressMessageBody();
        AudioFragmentSubmissionProgressMessageBody(std::vector<AudioFragmentSubmissionProgressMessageBodyChannel> channels);

        std::vector<AudioFragmentSubmissionProgressMessageBodyChannel> channels;
    };

    class AudioFragmentSubmissionProgressMessage {
    public:
        AudioFragmentSubmissionProgressMessage();
        AudioFragmentSubmissionProgressMessage(string message, AudioFragmentSubmissionProgressMessageBody body);

        string message;
        AudioFragmentSubmissionProgressMessageBody body;
    };
    
    void from_json(const json& j, AudioFragmentSubmissionProgressMessageBody& body);

    void from_json(const json& j, AudioFragmentSubmissionProgressMessage& message);

    void from_json(const json& j, AudioFragmentSubmissionProgressMessageBodyChannel& channel);
}

#endif
