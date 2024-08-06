#include "../../header/streaming/asr_job/stream_asr_job.h"

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
}
