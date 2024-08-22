#define POETICS_H_EXPORTS
#include "poetics.h"

int hello()
{
    std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
    return generator();
}

int test_audio_buffer()
{
    poetics::streaming::audio_buffer::AudioBuffer * audioBuffer = new poetics::streaming::audio_buffer::AudioBuffer();
    int bytePerSample = 2;
    int sampleRate = 8000;
    int byteSize = bytePerSample * sampleRate;
    char * data = new char[byteSize];
    // initialize data with 0 to 9
    for (int i = 0; i < byteSize; i++)
    {
        data[i] = i % 10;
    }

    const int RANGE = 25;
    for (int j = 0; j < RANGE; j++) {
        audioBuffer->AppendAudioData(data, byteSize);
    }

    for (int i = 0; i < RANGE; i++) {
        auto fragment = audioBuffer->GetFragmentAt(i, i == (RANGE - 1) ? true :false);
        fmt::print("fragment size: {}\n", fragment.first->size());
        audioBuffer->ReleaseAudioDataBefore(i);
    }
    
    auto fragment = audioBuffer->GetFragmentAt(12, false);  // fragment index is already released
    auto fragment2 = audioBuffer->GetFragmentAt(24, false);  // fragment index is not released yet

    return 1;
}