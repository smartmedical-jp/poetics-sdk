#ifndef STREAM_ASR_JOB_CORE_H
#define STREAM_ASR_JOB_CORE_H

#include <memory>
using std::unique_ptr;
using std::shared_ptr;
#include <vector>
using std::vector;
#include <algorithm>
#include <unordered_map>
using std::unordered_map;
#include <atomic>
using std::atomic;
#include <thread>
using std::jthread;
#include <mutex>

#include <Poco/Thread.h>
#include <Poco/SynchronizedObject.h>
#include <Poco/ScopedLock.h>
#include <Poco/Buffer.h>
#include <Poco/Mutex.h>

#include <Poco/Net/HTTPSClientSession.h>
using Poco::Net::HTTPSClientSession;
#include <Poco/Net/HTTPRequest.h>
using Poco::Net::HTTPRequest;
#include <Poco/Net/HTTPResponse.h>
using Poco::Net::HTTPResponse;
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/ConsoleCertificateHandler.h>
#include <Poco/Net/WebSocket.h>
using Poco::Net::WebSocket;
#include <Poco/Net/NetException.h>
#include <Poco/Crypto/OpenSSLInitializer.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

// For base64 encoding
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

#include <fmt/format.h>

#include "../../../include/stream_asr_job.h"
using namespace poetics::streaming::asr_job;

#include "../outgoing_message/create_stream_asr_job_message.h"
#include "../outgoing_message/close_stream_asr_job_message.h"
#include "../outgoing_message/submit_audio_fragment_message.h"
using namespace poetics::streaming::asr_job::outgoing_message;

#include "../incoming_message/job_detail_message.h"
#include "../incoming_message/job_status_updated_message.h"
#include "../incoming_message/audio_fragment_submission_progress_message.h"
#include "../incoming_message/error_message.h"
#include "../incoming_message/utterance_message.h"
#include "../incoming_message/temporary_utterance_message.h"
using namespace poetics::streaming::asr_job::incoming_message;

#include "../../audio_buffer/audio_buffer.h"
using namespace poetics::streaming::audio_buffer;

#include "audio_buffer_state.h"

#include "../../util/thread_safe_queue.h"

namespace poetics::streaming::asr_job::core {
    class StreamAsrJobCore : public StreamAsrJob, private Poco::Runnable
    {
    public:
        StreamAsrJobCore(const char * endpoint, const char * apiKey,
            const char * audioEncoding, int audioSampleRate, int channelCount, 
            bool enableDatalogging, const char * conversationTitle, vector<const char *>& channels);
        StreamAsrJobCore(const char * endpoint, const char * apiKey,
            const char * streamAsrJobID, int channelCount);
        ~StreamAsrJobCore();
    protected:
        virtual bool _connect() override;
        virtual bool _disconnect() override;
        virtual bool _isConnected() override;

        virtual bool _enqueueAudioData(int channelIndex, int audioFragmentIndex, const char* data, int dataSize) override;
        virtual void _setLastAudioFragmentIndex(int channelIndex, int audioFragmentIndex) override;
        virtual void _finishEnqueuingAudioData() override;
        virtual void _sendMessage(const char* data, int dataSize) override;
    private:
        unique_ptr<WebSocket> _webSocket;
        unique_ptr<HTTPSClientSession> _session;
        unique_ptr<HTTPRequest> _request;
        unique_ptr<HTTPResponse> _response;

        unique_ptr<Poco::Thread> _thread;
        unique_ptr<std::jthread> _requestThread;

        Poco::Mutex _mutex;
        std::mutex _requestMutex;

        shared_ptr<vector<int>> _lastAudioFragmentIndices;
        shared_ptr<vector<shared_ptr<AudioBuffer>>> _audioBuffers;
        shared_ptr<ThreadSafeQueue<std::pair<int, int>>> _audioDataQueue;
        const int DISCARD_FRAGMENT_INTERVAL = 50;
        
        // channels<audioFragmentIndex, AudioBufferState>
        shared_ptr<vector<unordered_map<int, AudioBufferState>>> _audioBufferStates;

        std::atomic<bool> _exit = false;

        StreamAsrJobCore();
        
        void run() override;
        bool _getResponse(Poco::Buffer<char>* _inBuffer);
        void _sendRequest(std::stop_token stopToken);
        void _sendAudioFragment();

        unique_ptr<json> _generateCreateStreamAsrJobMessage();
        unique_ptr<json> _generateCloseStreamAsrJobMessage();
        unique_ptr<json> _generateAudioFragmentMessage(int channelIndex, int audioFragmentIndex, shared_ptr<std::string> base64AudioData);
        unique_ptr<json> _processIncomingMessage(json &j);

        void _processProgressMessage(AudioFragmentSubmissionProgressMessage &progressMessage);

        std::shared_ptr<std::string> _base64Encode(const char * data, int dataSize);
        std::shared_ptr<std::string> _base64Encode(vector<char>* data);
    };
}

#endif
