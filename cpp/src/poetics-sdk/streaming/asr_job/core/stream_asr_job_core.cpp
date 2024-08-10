#include "stream_asr_job_core.h"

namespace poetics::streaming::asr_job::core {
    template <typename T>
    StreamAsrJobCore<T>::StreamAsrJobCore(IConnection<T> conn, int channelCount)
    {
        _channelCount = channelCount;
        _conn = conn;
    }

    template <typename T>
    CreateStreamAsrJobMessage StreamAsrJobCore<T>::buildCreateStreamAsrJobMessage(CreateStreamAsrJobOptions options)
    {
        return CreateStreamAsrJobMessage();
    }

    template <typename T>
    void StreamAsrJobCore<T>::createStreamAsrJob(CreateStreamAsrJobOptions options)
    {
    }

    template <typename T>
    void poetics::streaming::asr_job::core::StreamAsrJobCore<T>::connectToStreamAsrJob(ConnectToStreamAsrJobOptions options)
    {
    }
    
    template <typename T>
    void StreamAsrJobCore<T>::onReconnect()
    {
    }

    template <typename T>
    void StreamAsrJobCore<T>::onErrorMessage(ErrorMessage msg)
    {
    }

    template <typename T>
    void StreamAsrJobCore<T>::onJobDetailMessage(JobDetailMessage msg)
    {
    }

    template <typename T>
    void StreamAsrJobCore<T>::onJobStatusUpdatedMessage(JobStatusUpdatedMessage msg)
    {
    }

    template <typename T>
    void StreamAsrJobCore<T>::onAudioFragmentSubmissionProgressMessage(AudioFragmentSubmissionProgressMessage msg)
    {
    }

    template <typename T>
    void StreamAsrJobCore<T>::onTemporaryUtteranceMessage(TemporaryUtteranceMessage msg)
    {
    }

    template <typename T>
    void StreamAsrJobCore<T>::onUtteranceMessage(UtteranceMessage msg)
    {
    }

    template <typename T>
    void StreamAsrJobCore<T>::onReceiveMessageTimeout()
    {
    }

    template <typename T>
    void StreamAsrJobCore<T>::enqueueAudioData(int channelIndex, char *data)
    {
    }

    template <typename T>
    void StreamAsrJobCore<T>::finishEnqueuingAudioData()
    {
    }

    template <typename T>
    std::pair<bool, poetics::Error> StreamAsrJobCore<T>::isAllAudioDataSent()
    {
        return std::pair<bool, poetics::Error>();
    }
    
    template <typename T>
    std::pair<bool, poetics::Error> StreamAsrJobCore<T>::step()
    {
        return std::pair<bool, poetics::Error>();
    }

    template <typename T>
    void StreamAsrJobCore<T>::setOnUtteranceFunc(std::function<void(Utterance)> func)
    {
    }

    template <typename T>
    std::pair<StreamAsrJobDetail, poetics::Error> StreamAsrJobCore<T>::jobDetail()
    {
        return std::pair<StreamAsrJobDetail, poetics::Error>();
    }
}
