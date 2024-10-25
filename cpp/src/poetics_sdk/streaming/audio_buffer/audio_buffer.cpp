#include "audio_buffer.h"

namespace poetics::streaming::audio_buffer {
    AudioBuffer::AudioBuffer(int bytesPerFragment /* = DEFAULT_BYTE_SIZE */) 
    {
        _bytesPerFragment = bytesPerFragment;
        _fragments = std::make_unique<deque<audio_buffer_t>>();
    }

    AudioBuffer::~AudioBuffer() {
    }

    void AudioBuffer::AppendAudioData(int fragmentIndex, const char* data, size_t data_size) {
        std::lock_guard<std::mutex> lock(mutex);
        auto fragment = new std::vector<char>;
        fragment->reserve(data_size);
        std::copy(data, data + data_size, std::back_inserter(*fragment));
        _fragments->push_back(audio_buffer_t(fragmentIndex, fragment));
    }

    shared_ptr<vector<char>> AudioBuffer::GetFragmentAt(int fragmentIndex, bool isRecordingFinished) {
        // std::lock_guard<std::mutex> lock(mutex);

        auto container = *_fragments;
        // Temporary optimization until you don't use ReleaseFragmentAt().
        if (container.size() > fragmentIndex) {
            if (container[fragmentIndex].first == fragmentIndex) {
                return container[fragmentIndex].second;
            }
        }

        for (const auto& fragment : container) {
            if (fragment.first == fragmentIndex) {
                return fragment.second;
            }
        }

        return nullptr;
    }
    
    void AudioBuffer::ReleaseFragmentUntil(int fragmentIndex, unsigned int distance) {
        // Clear fragments until fragmentIndex using reverse iterator
        unsigned int count = 0;
        for (auto it = _fragments->rbegin(); it != _fragments->rend(); it++) {
            if (it->first <= fragmentIndex) {
                it->second->clear();
                count++;
            }
            if (count == distance) {
                break;
            }
        }
    }

    deque<audio_buffer_t>* AudioBuffer::GetFragments() { 
        return _fragments.get(); 
    }
}
