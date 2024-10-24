#ifndef AUDIO_BUFFER_STATE_H
#define AUDIO_BUFFER_STATE_H

namespace poetics::streaming::asr_job::core {
    enum class AudioBufferState: int
    {
        ENQUEUED,
        SENDING,
        SENT
    };
}

#endif
