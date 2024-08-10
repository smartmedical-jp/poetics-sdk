#include "stream_asr_job.h"

namespace poetics::streaming::asr_job {
    template <typename T>
    StreamAsrJob<T>::StreamAsrJob(string url, int channelCount, std::function<void(unique_ptr<StreamAsrJobCore<T>>)> init)
    {
    }

    template <typename T>
    unique_ptr<StreamAsrJob<T>> StreamAsrJob<T>::CreateStreamAsrJob(string url, CreateStreamAsrJobOptions options)
    {
        return unique_ptr<StreamAsrJob<T>>();
    }

    template <typename T>
    unique_ptr<StreamAsrJob<T>> StreamAsrJob<T>::ConnectToStreamAsrJob(string url, ConnectToStreamAsrJobOptions options)
    {
        return unique_ptr<StreamAsrJob<T>>();
    }

    template <typename T>
    Error StreamAsrJob<T>::processMessage(string msgStr)
    {
        return Error("not implemented");
    }

    template <typename T>
    void StreamAsrJob<T>::run()
    {
    }

    template <typename T>
    void StreamAsrJob<T>::subscribeUtterance()
    {
    }
    
    template <typename T>
    Error StreamAsrJob<T>::enqueueAudioData(int channelIndex, vector<uint8_t> audioData)
    {
        return Error("not implemented");
    }

    template <typename T>
    void StreamAsrJob<T>::finishEnqueuingAudioData()
    {
    }

    template <typename T>
    optional<StreamAsrJobDetail> StreamAsrJob<T>::jobDetail()
    {
        return std::nullopt;
    }

    template <typename T>
    Error StreamAsrJob<T>::Err()
    {
        return Error("not implemented");
    }
}
