#include "streaming_client.h"

namespace poetics::streaming {
    StreamingClient::StreamingClient(StreamingClientOptions options)
    {
        url = fmt::format("{}?api_key={}", options.getEndPoint(), options.getApiKey());
    }

    StreamAsrJob StreamingClient::createStreamAsrJob(CreateStreamAsrJobOptions options)
    {
        return StreamAsrJob();
    }

    StreamAsrJob StreamingClient::connectToStreamAsrJob(ConnectToStreamAsrJobOptions options)
    {
        return StreamAsrJob();
    }
}
