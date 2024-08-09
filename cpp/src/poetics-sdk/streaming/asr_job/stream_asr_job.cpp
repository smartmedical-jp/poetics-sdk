#include "stream_asr_job.h"

namespace poetics::streaming::asr_job {
    StreamAsrJob::StreamAsrJob(string url, int channelCount, std::function<void(unique_ptr<StreamAsrJobCore>)> init)
    {
    }
    unique_ptr<StreamAsrJob> StreamAsrJob::CreateStreamAsrJob(string url, CreateStreamAsrJobOptions options)
    {
        return unique_ptr<StreamAsrJob>();
    }

    unique_ptr<StreamAsrJob> StreamAsrJob::ConnectToStreamAsrJob(string url, ConnectToStreamAsrJobOptions options)
    {
        return unique_ptr<StreamAsrJob>();
    }

    Error StreamAsrJob::processMessage(string msgStr)
    {
        return Error("not implemented");
    }

    void StreamAsrJob::run()
    {
    }

    void StreamAsrJob::subscribeUtterance()
    {
    }
    
    Error StreamAsrJob::enqueueAudioData(int channelIndex, vector<uint8_t> audioData)
    {
        return Error("not implemented");
    }

    void StreamAsrJob::finishEnqueuingAudioData()
    {
    }

    optional<StreamAsrJobDetail> StreamAsrJob::jobDetail()
    {
        return std::nullopt;
    }

    Error StreamAsrJob::Err()
    {
        return Error("not implemented");
    }
}
