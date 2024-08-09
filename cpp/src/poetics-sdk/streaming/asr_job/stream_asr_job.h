#ifndef STREAM_ASR_JOB_H
#define STREAM_ASR_JOB_H

#include <string>
using std::string;
#include <functional>
#include <memory>
using std::unique_ptr;
#include <vector>
using std::vector;
#include <optional>
using std::optional;

#include "core/stream_asr_job_core.h"
#include "core/options.h"
#include "core/stream_asr_job_detail.h"
#include "core/utterance.h"
#include "../websocket_connection/websocket_connection.h"
#include "../../poetics_error.h"
using namespace poetics::streaming::asr_job::core;

namespace poetics::streaming::asr_job {
    template <typename T>
    class StreamAsrJob
    {
    public:
        StreamAsrJob(){}; // TODO: Should be removed
        StreamAsrJob(string url, int channelCount, std::function<void(unique_ptr<StreamAsrJobCore>)> init);
        unique_ptr<StreamAsrJob> CreateStreamAsrJob(string url, CreateStreamAsrJobOptions options);
        unique_ptr<StreamAsrJob> ConnectToStreamAsrJob(string url, ConnectToStreamAsrJobOptions options);
    private:
        poetics::streaming::websocket_connection::WebsocketConnection<T> conn;
        //StreamAsrJobCore core
        Utterance utterance; // is chan in Golang
        Error error;

        Error   processMessage(string msgStr);
        void    run();
        void    subscribeUtterance();
        Error   enqueueAudioData(int channelIndex, vector<uint8_t> audioData);
        void    finishEnqueuingAudioData();
        optional<StreamAsrJobDetail> jobDetail();
        Error Err();
    };
}


#endif