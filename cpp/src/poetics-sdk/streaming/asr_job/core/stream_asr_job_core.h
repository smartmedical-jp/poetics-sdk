#ifndef STREAM_ASR_JOB_CORE_H
#define STREAM_ASR_JOB_CORE_H

#include <functional>
#include <vector>
#include <utility>

#include "../../audio_buffer/audio_buffer.h"
using poetics::streaming::audio_buffer::AudioBuffer;
#include "stream_asr_job_detail.hpp"
#include "utterance.hpp"
#include "connection.hpp"
#include "options.hpp"
using namespace poetics::streaming::asr_job::core;
#include "../incoming_message/audio_fragment_submission_progress_message.hpp"
#include "../incoming_message/error_message.hpp"
#include "../incoming_message/job_detail_message.hpp"
#include "../incoming_message/job_status_updated_message.hpp"
#include "../incoming_message/temporary_utterance_message.hpp"
#include "../incoming_message/utterance_message.hpp"
using namespace poetics::streaming::asr_job::incoming_message;
#include "../outgoing_message/create_stream_asr_job_message.hpp"
using namespace poetics::streaming::asr_job::outgoing_message;
#include "../../../poetics_error.h"

namespace poetics::streaming::asr_job::core {
    template <typename T>
    class StreamAsrJobCore
    {
    public:
        StreamAsrJobCore(IConnection<T> conn, int channelCount);
        CreateStreamAsrJobMessage buildCreateStreamAsrJobMessage(CreateStreamAsrJobOptions options);
    private:
        void createStreamAsrJob(CreateStreamAsrJobOptions options);
        void connectToStreamAsrJob(ConnectToStreamAsrJobOptions options);
        void onReconnect();
        void onErrorMessage(ErrorMessage msg);
        void onJobDetailMessage(JobDetailMessage msg);
        void onJobStatusUpdatedMessage(JobStatusUpdatedMessage msg);
        void onAudioFragmentSubmissionProgressMessage(AudioFragmentSubmissionProgressMessage msg);
        void onTemporaryUtteranceMessage(TemporaryUtteranceMessage msg);
        void onUtteranceMessage(UtteranceMessage msg);
        void onReceiveMessageTimeout();
        void enqueueAudioData(int channelIndex, char* data);
        void finishEnqueuingAudioData();
        std::pair<bool, poetics::Error> isAllAudioDataSent();
        std::pair<bool, poetics::Error> step(); // (bool continueLoop, Error err)
        void setOnUtteranceFunc(std::function<void(Utterance)> func);
        std::pair<StreamAsrJobDetail, poetics::Error> jobDetail();

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