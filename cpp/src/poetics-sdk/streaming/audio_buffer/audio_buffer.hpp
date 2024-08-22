#ifndef STREAMING_AUDIO_BUFFER_H
#define STREAMING_AUDIO_BUFFER_H

#include <mutex>
#include <utility>
#include <vector>

#include "../../poetics_error.h"

namespace poetics::streaming::audio_buffer {
    class IAudioBuffer {
    public:
        virtual ~IAudioBuffer() = default;

        virtual void AppendAudioData(char* data, size_t data_size) = 0;
        virtual std::pair<char*, poetics::Error> GetFragmentAt(int fragmentIndex, bool isRecordingFinished) = 0;
        virtual void ReleaseAudioDataBefore(int fragmentIndex) = 0;
    };

    class AudioBuffer : public IAudioBuffer {
    public:
        AudioBuffer(int bytesPerFragment = 16000) 
        {
            _bytesPerFragment = bytesPerFragment;
            _bytes = new std::vector<char>();
        }

        ~AudioBuffer() {
            delete _bytes;
        }

        void AppendAudioData(char* data, size_t data_size) override {
            mutex.lock();
            if (data_size > 0)
            {
                for (int i = 0; i < data_size; i++)
                {
                    _bytes->push_back(data[i]);
                }
            }
            mutex.unlock();
        }
        std::pair<char*, poetics::Error> GetFragmentAt(int fragmentIndex, bool isRecordingFinished) override {
            return std::pair<char *, poetics::Error>();
        }
        void ReleaseAudioDataBefore(int fragmentIndex) override {

        }

        std::vector<char> * GetBytes() { return _bytes; }
    private:
        std::vector<char> * _bytes;
        int _fromFragmentIndex = 0;
        int _bytesPerFragment;
        int _discardableFragmentIndex = 0;
        std::mutex mutex;
    };
}

#endif