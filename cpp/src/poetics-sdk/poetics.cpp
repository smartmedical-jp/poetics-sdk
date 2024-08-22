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
    char * data = new char[16000];
    // initialize data with 0 to 9
    for (int i = 0; i < 16000; i++)
    {
        data[i] = i % 10;
    }

    audioBuffer->AppendAudioData(data, 16000);
    audioBuffer->AppendAudioData(data, 16000);

    return 1;
}