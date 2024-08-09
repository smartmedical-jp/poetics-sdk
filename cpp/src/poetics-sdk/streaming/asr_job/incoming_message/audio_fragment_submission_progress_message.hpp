#ifndef AUDIO_FRAGMENT_SUBMISSION_PROGRESS_MESSAGE_H
#define AUDIO_FRAGMENT_SUBMISSION_PROGRESS_MESSAGE_H

#include <string>
using std::string;
#include <vector>

namespace poetics::streaming::asr_job::incoming_message {
    class AudioFragmentSubmissionProgressMessageBodyChannel;
    class AudioFragmentSubmissionProgressMessageBody;

    class AudioFragmentSubmissionProgressMessage {
    public:
        AudioFragmentSubmissionProgressMessage(string message, AudioFragmentSubmissionProgressMessageBody body) : _message(message), _body(body) {}

        string getMessage() { return _message; }
        AudioFragmentSubmissionProgressMessageBody getBody() { return _body; }
    private:
        string _message;
        AudioFragmentSubmissionProgressMessageBody _body;
    };

    class AudioFragmentSubmissionProgressMessageBody {
    public:
        AudioFragmentSubmissionProgressMessageBody(std::vector<AudioFragmentSubmissionProgressMessageBodyChannel> channels) : _channels(channels) {}

        std::vector<AudioFragmentSubmissionProgressMessageBodyChannel> getChannels() { return _channels; }
    private:
        std::vector<AudioFragmentSubmissionProgressMessageBodyChannel> _channels;
    };

    class AudioFragmentSubmissionProgressMessageBodyChannel {
    public:
        AudioFragmentSubmissionProgressMessageBodyChannel(int channelIndex, int audioFragmentCount) : 
            _channelIndex(channelIndex), _audioFragmentCount(audioFragmentCount) {}
    private:
        int _channelIndex;
        int _audioFragmentCount;
    };
}

#endif