#include "audio_buffer.h"

void poetics::streaming::audio_buffer::AudioBuffer::AppendAudioData(char *data)
{
}

std::pair<char *, poetics::Error> poetics::streaming::audio_buffer::AudioBuffer::GetFragmentAt(int fragmentIndex, bool isRecordingFinished)
{
    return std::pair<char *, poetics::Error>();
}

void poetics::streaming::audio_buffer::AudioBuffer::ReleaseAudioDataBefore(int fragmentIndex)
{
}
