#ifndef STREAMING_AUDIO_BUFFER_H
#define STREAMING_AUDIO_BUFFER_H

#include <mutex>
#include <utility>

#include "../../poetics_error.h"

namespace poetics::streaming::audio_buffer {
    class IAudioBuffer {
    public:
        virtual ~IAudioBuffer();

        virtual void AppendAudioData(char* data) = 0;
        virtual std::pair<char*, poetics::Error> GetFragmentAt(int fragmentIndex, bool isRecordingFinished) = 0;
        virtual void ReleaseAudioDataBefore(int fragmentIndex) = 0;
    };

    class AudioBuffer : public IAudioBuffer {
    public:
        AudioBuffer(char* bytes, int fromFragmentIndex, int bytesPerFragment, int discardableFragmentIndex) 
        {
            _bytes = bytes;
            _fromFragmentIndex = fromFragmentIndex;
            _bytesPerFragment = bytesPerFragment;
            _discardableFragmentIndex = discardableFragmentIndex;
        }

        void AppendAudioData(char* data) override;
        std::pair<char*, poetics::Error> GetFragmentAt(int fragmentIndex, bool isRecordingFinished) override;
        void ReleaseAudioDataBefore(int fragmentIndex) override;
    private:
        char* _bytes;
        int _fromFragmentIndex;
        int _bytesPerFragment;
        int _discardableFragmentIndex;
        std::mutex mutex;
    };
}

#endif