#ifndef STREAM_ASR_JOB_CORE_H
#define STREAM_ASR_JOB_CORE_H

#include <functional>
#include <vector>

#include "../../audio_buffer/audio_buffer.h"
using poetics::streaming::audio_buffer::AudioBuffer;
#include "stream_asr_job_detail.hpp"
#include "utterance.hpp"
using namespace poetics::streaming::asr_job::core;

namespace poetics::streaming::asr_job::core {
    template <typename T>
    class StreamAsrJobCore
    {
    public:

    private:
        int _channelCount;
        std::vector<AudioBuffer> _audioBuffers;
        bool _isNextFragmentIndicesInitialized;
        std::vector<int> _nextFragmentIndices;
        bool _isJobDetailInitialized;
        StreamAsrJobDetail _jobDetail;
        bool _isNextFragmentSendable;
        bool _isEnqueuingAudioDataFinished;
        bool _isCloseJobMessageSent;
        int _errorMessageCount;
        int _nextSendingFragmentIndex;
        std::function<void(Utterance)> _onUtteranceFunc;
    };
}

#endif