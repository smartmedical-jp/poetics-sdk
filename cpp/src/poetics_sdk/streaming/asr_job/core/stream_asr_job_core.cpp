#include "stream_asr_job_core.h"

namespace poetics::streaming::asr_job::core {
    StreamAsrJobCore::StreamAsrJobCore(
        string endpoint, string apiKey,
        string audioEncoding, int audioSampleRate, int channelCount, 
        bool enableDatalogging, string conversationTitle, vector<string>& channels)
    {
        _endpoint = endpoint;
        _apiKey = apiKey;

        _audioEncoding = audioEncoding;
        _audioSampleRate = audioSampleRate;
        _channelCount = channelCount;
        _enableDatalogging = enableDatalogging;
        _conversationTitle = conversationTitle;
        _channels = std::make_unique<vector<string>>(channels);

        _audioBuffers = std::make_shared<vector<shared_ptr<AudioBuffer>>>();
        for (int i = 0; i < _channelCount; i++) {
            _audioBuffers->push_back(std::make_shared<AudioBuffer>());
        }
        _audioDataQueue = std::make_shared<ThreadSafeQueue<std::pair<int, int>>>();

        _audioBufferStates = std::make_shared<vector<unordered_map<int, AudioBufferState>>>();
        for (int i = 0; i < _channelCount; i++) {
            _audioBufferStates->push_back(unordered_map<int, AudioBufferState>());
        }

        _lastAudioFragmentIndices = std::make_shared<vector<int>>();

        for (int i = 0; i < _channelCount; i++) {
            _lastAudioFragmentIndices->push_back(-1);
        }
    }

    StreamAsrJobCore::StreamAsrJobCore(string endpoint, string apiKey,
        string streamAsrJobID, int channelCount)
    {
        _endpoint = endpoint;
        _apiKey = apiKey;

        _streamAsrJobID = streamAsrJobID;
        _channelCount = channelCount;
    }

    StreamAsrJobCore::StreamAsrJobCore(/* args */)
    {
    }
    
    StreamAsrJobCore::~StreamAsrJobCore()
    {
    }

    bool StreamAsrJobCore::_connect()
    {
        int flags;
        string dir = "/v1?api_key=" + _apiKey;

        // /*
        // // Use VERIFY_STRICT if you have acquired a root certificate
        // SSLInitializer sslInitializer;

        // Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> ptrCert = new Poco::Net::ConsoleCertificateHandler(false);
        // Poco::Net::Context::Ptr ptrContext = new Poco::Net::Context(
        //     Poco::Net::Context::CLIENT_USE, "", "", "rootcert.pem", Poco::Net::Context::VERIFY_STRICT, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
        // Poco::Net::SSLManager::instance().initializeClient(0, ptrCert, ptrContext);
        // Poco::Net::HTTPSClientSession session(endpoint, PORT);
        // */
        Poco::Net::Context::Ptr ptrContext = new Poco::Net::Context(
        Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
        _session = std::make_unique<HTTPSClientSession>(_endpoint, 443, ptrContext);
        _session->setTimeout(Poco::Timespan(30, 0));

        _request = std::make_unique<HTTPRequest>(HTTPRequest::HTTP_GET, dir, HTTPRequest::HTTP_1_1);
        _response = std::make_unique<HTTPResponse>();

        try {
            _webSocket = std::make_unique<WebSocket>(*_session, *_request, *_response);
            char buffer[1024] = {
                0,
            };

            string payload = _generateCreateStreamAsrJobMessage()->dump();

            _sendMessage(payload.c_str(), payload.size());
        }
        catch (Poco::Exception& e) {
            onErrorReceived(e.displayText());

            return false;
        }

        _thread = std::make_unique<Poco::Thread>();
        _thread->start(*this);
        _requestThread = std::make_unique<std::jthread>([this](std::stop_token s) { _sendRequest(s); });

        return true;
    }

    bool StreamAsrJobCore::_disconnect()
    {
        try {
            if (_webSocket->impl()->initialized()) {
                _webSocket->shutdown();
            }
        } catch (Poco::Exception& e) {
            onErrorReceived(e.displayText());
        }

        _thread->join();

        if (_webSocket->impl()->initialized()) {
            _webSocket->close();
        }

        return true;
    }

    bool StreamAsrJobCore::_isConnected()
    {
        if (!_webSocket) {
            if (_webSocket->impl()->initialized()) {
                return true;
            }
        }
        return false;
    }

    bool StreamAsrJobCore::_enqueueAudioData(int channelIndex, int audioFragmentIndex, const char* data, int dataSize)
    {
        if (_exit.load(std::memory_order_acquire)) {
            return false;
        }
        if (dataSize > 21000) {
            // Maximum JSON message size is 32kB.
            // So limiting the raw data size to 21kB, which encoded data size will be 28kB.
            // (4KB for the other data fields, 1kB = 1000B)
            onErrorReceived("Raw data size should not be greater than 21kB");
            return false;
        } else if (dataSize <= 0) {
            onErrorReceived("Raw data size should be greater than 0");
            return false;
        }

        onDebugMessageReceived(fmt::format("Enqueuing data for channel index {}, audio fragment index {}", channelIndex, audioFragmentIndex));

        auto channelAudioBuffer = (*_audioBuffers)[channelIndex];
        channelAudioBuffer->AppendAudioData(audioFragmentIndex, data, dataSize);

        return true;
    }

    void StreamAsrJobCore::_setLastAudioFragmentIndex(int channelIndex, int audioFragmentIndex)
    {
        onDebugMessageReceived(fmt::format("Setting last audio fragment index for channel index {}, audio fragment index {}", channelIndex, audioFragmentIndex));
        (*_lastAudioFragmentIndices)[channelIndex] = audioFragmentIndex;
    }

    void StreamAsrJobCore::_finishEnqueuingAudioData()
    {
        // Check if all audio data is sent
        // onMessageReceived("Checking if all audio data is sent");
        std::atomic<bool> allAudioDataSent;
        do {
            allAudioDataSent.store(true, std::memory_order_release);
            if (_exit.load(std::memory_order_acquire)) {
                // This can be true when the server closes the connection before all audio data is sent
                break;
            }
            for (int i = 0; i < _channelCount; i++) {
                // Check _lastAudioFragmentIndices
                auto& lastAudioFragmentIndex = _lastAudioFragmentIndices->at(i);
                auto& audioBufferStates = _audioBufferStates->at(i);
                if (!audioBufferStates.contains(lastAudioFragmentIndex)) {
                    allAudioDataSent.store(false, std::memory_order_release);
                    break;
                } else {
                    if (audioBufferStates[lastAudioFragmentIndex] != AudioBufferState::SENT) {
                        // onDebugMessageReceived(fmt::format("Pending data: Channel {}, AudioFragment {}", i, lastAudioFragmentIndex));
                        allAudioDataSent.store(false, std::memory_order_release);
                        break;
                    }
                }

                if (!allAudioDataSent.load(std::memory_order_acquire)) {
                    // Escape the for loop since there is still pending data
                    // (Which means do-while loop will be executed again)
                    break;
                }
            }
        }
        while (!allAudioDataSent.load(std::memory_order_acquire));
        cout << "allAudioDataSent: " << allAudioDataSent << endl;
        _requestThread->request_stop();

        auto jsonMessage = _generateCloseStreamAsrJobMessage();
        auto payload = jsonMessage->dump();
        _sendMessage(payload.c_str(), payload.size());
    }

    std::shared_ptr<std::string> StreamAsrJobCore::_base64Encode(const char* data, int dataSize)
    {
        BIO *bio, *b64;
        BUF_MEM *bufferPtr;

        b64 = BIO_new(BIO_f_base64());
        bio = BIO_new(BIO_s_mem());
        bio = BIO_push(b64, bio);

        BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
        BIO_write(bio, data, dataSize);
        BIO_flush(bio);
        BIO_get_mem_ptr(bio, &bufferPtr);

        auto result = std::make_shared<std::string>(bufferPtr->data, bufferPtr->length);
        BIO_free_all(bio);

        return result;
    }

    std::shared_ptr<std::string> StreamAsrJobCore::_base64Encode(vector<char> *data)
    {
        return _base64Encode(data->data(), data->size());
    }

    void StreamAsrJobCore::_sendMessage(const char* data, int dataSize)
    {
        auto bytesSent =_webSocket->sendFrame(data, dataSize, WebSocket::FRAME_TEXT);
        if (bytesSent > 0) {
            onDebugMessageReceived(fmt::format("Sent data: {}", string(data, dataSize)));
        } else {
            onErrorReceived(fmt::format("Failed to send data: {}", string(data, dataSize)));
        }
    }

    void StreamAsrJobCore::run()
    {
        try {
            Poco::Buffer<char>* _inBuffer;
            _inBuffer = new Poco::Buffer<char>(0);
            while (!_exit.load(std::memory_order_acquire)) {
                // onDebugMessageReceived(fmt::format("run() started"));
                // Receiving data from the server
                if (_getResponse(_inBuffer)) {
                    // onDebugMessageReceived(fmt::format("Received data: {}", string(_inBuffer->begin(), _inBuffer->end())));
                    string response = string(_inBuffer->begin(), _inBuffer->end());
                    // check if the response is ETX
					if (response[0] == '\x03') {
                        onDebugMessageReceived("Server closed the connection.");
						_exit.store(true, std::memory_order_release);
						break;
					}
                    try {
                        auto json = json::parse(response);
                        auto _ = _processIncomingMessage(json);
                        if (json["message"] == "job_status_updated") {
                            auto& body = json["Body"];
                            if (body["status"] == "completed") {
                                onDebugMessageReceived(fmt::format("Job completed: {}", getStreamAsrJobID()));
                                _exit.store(true, std::memory_order_release);
                                break;
                            }
                        }
                    } catch (json::parse_error& e) {
                        onErrorReceived(e.what());
                        _exit.store(true, std::memory_order_release);
                        break;
                    }
                    // If we want to reuse the buffer, we need to clear it by calling resize(0).
                    // This prevents the buffer accumulating data from previous responses.
                    _inBuffer->resize(0);
                }
                // onDebugMessageReceived(fmt::format("run() ended"));
            }
            // onDebugMessageReceived(fmt::format("StreamAsrJobCore is exiting"));
        } catch (Poco::TimeoutException& e) {
            onErrorReceived(e.displayText());
            _exit.store(true, std::memory_order_release);
        } catch (Poco::Exception& e) {
            onErrorReceived(e.displayText());
            _exit.store(true, std::memory_order_release);
        } catch (std::exception& e) {
            onErrorReceived(e.what());
            _exit.store(true, std::memory_order_release);
        }
    }

    bool StreamAsrJobCore::_getResponse(Poco::Buffer<char>* _inBuffer)
    {
        int flags;
        onDebugMessageReceived(fmt::format("Waiting for response..."));
        int recvDataSize = _webSocket->receiveFrame(*_inBuffer, flags);
        if (recvDataSize > 0) {
            return true;
        }
        return false;
    }

    void StreamAsrJobCore::_sendRequest(std::stop_token stopToken)
    {
        // std::lock_guard<std::mutex> lock(_requestMutex);
        // Sending data to the server
        while (!_exit.load(std::memory_order_acquire) && !stopToken.stop_requested()) {
            if (!_audioDataQueue->empty()) {
                // onDebugMessageReceived(fmt::format("AudioDataQueue is not empty. Sending audio fragment..."));
                _sendAudioFragment();
            }
            std::this_thread::yield();
        }
    }

    void StreamAsrJobCore::_sendAudioFragment()
    {
        Poco::Mutex::ScopedLock lock(_mutex);
        if (_audioBufferStates->empty()) {
            return;
        }

        auto audioFragment = _audioDataQueue->front();
        auto channelIndex = audioFragment.value().first;
        auto audioFragmentCount = audioFragment.value().second;
        auto lastAudioFragmentCount = (*_lastAudioFragmentIndices)[channelIndex];
        bool isRecordingFinished = audioFragmentCount == lastAudioFragmentCount;
        // fmt::println("lastAudioFragmentCount = {}, audioFragmentCount = {}", lastAudioFragmentCount, audioFragmentCount);
        if (lastAudioFragmentCount > -1 && lastAudioFragmentCount < audioFragmentCount) {
            // Audio fragment count is out of order. In general case, this means that every audio fragment has been sent.
            _audioDataQueue->pop();
            return;
        }

        if ((*_audioBufferStates)[channelIndex][audioFragmentCount] == AudioBufferState::ENQUEUED) {
            _audioDataQueue->pop();

            auto& audioBuffer = (*_audioBuffers)[channelIndex];
            auto data = audioBuffer->GetFragmentAt(audioFragmentCount, isRecordingFinished);
            if (data == nullptr) {
                // data has not been enqueued yet
                onDebugMessageReceived(fmt::format("AudioBuffer {} is not filled yet. Waiting...", audioFragmentCount));
                while (data == nullptr) {
                    std::this_thread::yield();
                    data = audioBuffer->GetFragmentAt(audioFragmentCount, isRecordingFinished);
                }
            }
            
            auto encodedAudioData = _base64Encode(data.get());
            // Print only the first 32 characters
            onDebugMessageReceived(fmt::format("Sending audio fragment {} in channel {} with encoded data: {}...", 
                audioFragmentCount, channelIndex, encodedAudioData->substr(0, 32)));
            auto jsonMessage = _generateAudioFragmentMessage(channelIndex, audioFragmentCount, encodedAudioData);
            auto jsonString = jsonMessage->dump();
            _sendMessage(jsonString.c_str(), jsonString.length());

            ((*_audioBufferStates)[channelIndex])[audioFragmentCount] = AudioBufferState::SENDING;
        }
        else {
            onDebugMessageReceived(fmt::format("AudioBufferState is not ENQUEUED. AudioFragmentCount: {}", audioFragmentCount));
        }
    }

    unique_ptr<json> StreamAsrJobCore::_processIncomingMessage(json &j)
    {
        if (j["message"] == "job_detail") {
            JobDetailMessage jobDetailMessage;
            from_json(j, jobDetailMessage);
            onMessageReceived(j.dump());

            _streamAsrJobID = jobDetailMessage.body.streamAsrJobID;
            onDebugMessageReceived(fmt::format("Set StreamAsrJobID as {}", _streamAsrJobID));
        } else if (j["message"] == "audio_fragment_submission_progress") {
            AudioFragmentSubmissionProgressMessage progressMessage;
            from_json(j, progressMessage);
            onMessageReceived(j.dump());

            _processProgressMessage(progressMessage);
        } else if (j["message"] == "job_status_updated") {
            JobStatusUpdatedMessage jobStatusUpdatedMessage;
            from_json(j, jobStatusUpdatedMessage);
            onMessageReceived(j.dump());
        } else if (j["message"] == "utterance") {
            UtteranceMessage utteranceMessage;
            from_json(j, utteranceMessage);
            onMessageReceived(j.dump());
        } else if (j["message"] == "temporary_utterance") {
            TemporaryUtteranceMessage temporaryUtteranceMessage;
            from_json(j, temporaryUtteranceMessage);
            onMessageReceived(j.dump());
        } else if (j["message"] == "error") {
            ErrorMessage errorMessage;
            from_json(j, errorMessage);
            _exit.store(true, std::memory_order_release);
            onErrorReceived(j.dump());
        }
        else {
            onErrorReceived(fmt::format("Unknown message: {}", j.dump()));
        }

        return std::make_unique<json>(j);
    }

    void StreamAsrJobCore::_processProgressMessage(AudioFragmentSubmissionProgressMessage &progressMessage)
    {
        // Poco::Mutex::ScopedLock lock(_mutex);

        for (auto &channel : progressMessage.body.channels)
        {
            if (_audioBuffers->size() < channel.channelIndex) {
                onErrorReceived(fmt::format("Channel index {} is out of range", channel.channelIndex));
                continue;
            }

            auto& audioBufferStates = _audioBufferStates->at(channel.channelIndex);
            if (audioBufferStates.contains(channel.audioFragmentCount)) {
                // Do nothing because the map should be unique
                continue;
            }
            else {
                // Change the state of the audio buffers which count is less than the current audio fragment to SENT 
                auto currentCount = channel.audioFragmentCount;
                for (auto i = 0; i < currentCount; i++) {
                    if (audioBufferStates.contains(i)) {
                        if (audioBufferStates[i] != AudioBufferState::SENT) {
                            audioBufferStates[i] = AudioBufferState::SENT;
                        }
                    }
                }

                audioBufferStates.insert(std::make_pair(channel.audioFragmentCount, AudioBufferState::ENQUEUED));
                _audioDataQueue->push(std::make_pair(channel.channelIndex, channel.audioFragmentCount));
                onDebugMessageReceived(fmt::format("Waiting for audio fragment {} in channel {}", channel.audioFragmentCount, channel.channelIndex));
            }
        }
    }

    unique_ptr<json> StreamAsrJobCore::_generateCreateStreamAsrJobMessage()
    {
        vector<CreateStreamAsrJobMessageBodyChannel> channels;
        for (int i = 0; i < _channelCount; i++)
        {
            channels.push_back(CreateStreamAsrJobMessageBodyChannel(_channels->at(i)));
        }
        CreateStreamAsrJobMessageBody body(
            getEnableDatalogging(), getConversationTitle(),
            getAudioEncoding(), getAudioSampleRate(), getChannelCount(), 
            channels
        );
        CreateStreamAsrJobMessage message("create_stream_asr_job", body);

        auto j = std::make_unique<json>(message);
        return j;
    }

    unique_ptr<json> StreamAsrJobCore::_generateCloseStreamAsrJobMessage()
    {
        auto message = CloseStreamAsrJobMessage::CreateDefault();
        auto j = std::make_unique<json>(message);
        return j;
    }

    unique_ptr<json> StreamAsrJobCore::_generateAudioFragmentMessage(int channelIndex, int audioFragmentIndex, shared_ptr<string> base64AudioData)
    {
        auto messageBody = SubmitAudioFragmentMessageBody(channelIndex, audioFragmentIndex, base64AudioData->data());
        SubmitAudioFragmentMessage message("submit_audio_fragment", messageBody);
        auto j = std::make_unique<json>(message);
        return j;
    }
}
