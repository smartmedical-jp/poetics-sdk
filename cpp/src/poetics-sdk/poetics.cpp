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

    audioBuffer->AppendAudioData(data, byteSize);
    audioBuffer->AppendAudioData(data, byteSize);

    auto fragment1 = audioBuffer->GetFragmentAt(0, false);
    auto fragment2 = audioBuffer->GetFragmentAt(1, true);

    audioBuffer->ReleaseAudioDataBefore(1);

    return 1;
}