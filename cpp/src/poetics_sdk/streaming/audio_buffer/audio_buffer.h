#ifndef STREAMING_AUDIO_BUFFER_H
#define STREAMING_AUDIO_BUFFER_H

#include <mutex>
#include <utility>
using std::pair;
#include <vector>
using std::vector;
#include <deque>
using std::deque;
#include <memory>
using std::unique_ptr;
using std::shared_ptr;

#include <fmt/format.h>

#include "../../poetics_error.h"

#define DEFAULT_BYTE_SIZE 21000
#define DISCARD_THRESHOLD_FRAGMENTS 10

namespace poetics::streaming::audio_buffer {
    // pair<fragmentIndex, buffer>
    typedef pair<int, shared_ptr<vector<char>>> audio_buffer_t;

    // Declaration of IAudioBuffer
    class IAudioBuffer {
    public:
        virtual ~IAudioBuffer() = default;

        virtual void AppendAudioData(int fragmentIndex, const char* data, size_t data_size) = 0;
        virtual shared_ptr<vector<char>> GetFragmentAt(int fragmentIndex, bool isRecordingFinished) = 0;
        virtual void ReleaseFragmentUntil(int fragmentIndex, unsigned int distance) = 0;
    };

    // Declaration of AudioBuffer
    class AudioBuffer : public IAudioBuffer {
    public:
        AudioBuffer(int bytesPerFragment = DEFAULT_BYTE_SIZE);
        ~AudioBuffer();

        void AppendAudioData(int fragmentIndex, const char* data, size_t data_size) override;
        shared_ptr<vector<char>> GetFragmentAt(int fragmentIndex, bool isRecordingFinished) override;
        void ReleaseFragmentUntil(int fragmentIndex, unsigned int distance) override;
        deque<audio_buffer_t>* GetFragments();
    private:
        int _bytesPerFragment;
        shared_ptr<deque<audio_buffer_t>> _fragments;
        std::mutex mutex;
    };
}

#endif
