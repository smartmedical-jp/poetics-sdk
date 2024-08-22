#ifndef STREAMING_AUDIO_BUFFER_H
#define STREAMING_AUDIO_BUFFER_H

#include <mutex>
#include <utility>
#include <vector>
#include <deque>
#include <memory>
using std::unique_ptr;

#include "../../poetics_error.h"

#define DEFAULT_BYTE_SIZE 16000

namespace poetics::streaming::audio_buffer {
    class IAudioBuffer {
    public:
        virtual ~IAudioBuffer() = default;

        virtual void AppendAudioData(char* data, size_t data_size) = 0;
        virtual std::pair<std::vector<char> *, poetics::Error> GetFragmentAt(int fragmentIndex, bool isRecordingFinished) = 0;
        virtual void ReleaseAudioDataBefore(int fragmentIndex) = 0;
    };

    class AudioBuffer : public IAudioBuffer {
    public:
        AudioBuffer(int bytesPerFragment = DEFAULT_BYTE_SIZE) 
        {
            _bytesPerFragment = bytesPerFragment;
            _bytes = std::make_unique<std::deque<std::vector<char> *>>();
        }

        ~AudioBuffer() {
        }

        void AppendAudioData(char* data, size_t data_size) override {
            mutex.lock();
            auto fragment = new std::vector<char>;
            fragment->reserve(_bytesPerFragment);
            if (data_size > 0)
            {
                std::copy(data, data + data_size, std::back_inserter(*fragment));
            }
            _bytes->push_back(fragment);
            mutex.unlock();
        }

        std::pair<std::vector<char> *, poetics::Error> GetFragmentAt(int fragmentIndex, bool isRecordingFinished) override {
            mutex.lock();

            std::vector<char> * fragment;
            if (fragmentIndex < _fromFragmentIndex) {
                return std::pair(nullptr, poetics::Error(fmt::format("fragment {} is already released", fragmentIndex)));
            }

            if (fragmentIndex > _bytes->size() - 1) {
                return std::pair(nullptr, poetics::Error(fmt::format("fragment {} is not found", fragmentIndex)));
            }

            fragment = _bytes->at(fragmentIndex);

            mutex.unlock();
            return std::pair<std::vector<char> *, poetics::Error>(fragment, poetics::Error());
        }
        
        void ReleaseAudioDataBefore(int fragmentIndex) override {
            mutex.lock();
            while (_bytes->size() > 0 && _bytes->front()->size() == 0) {
                delete _bytes->front();
                _bytes->pop_front();
            }
            while (_bytes->size() > fragmentIndex) {
                delete _bytes->front();
                _bytes->pop_front();
            }
            _fromFragmentIndex = fragmentIndex;
            mutex.unlock();
        }

        std::deque<std::vector<char> *> * GetBytes() { return _bytes.get(); }
    private:
        unique_ptr<std::deque<std::vector<char> *>> _bytes;
        int _fromFragmentIndex = 0;
        int _bytesPerFragment;
        int _discardableFragmentIndex = 0;
        std::mutex mutex;
    };
}

#endif