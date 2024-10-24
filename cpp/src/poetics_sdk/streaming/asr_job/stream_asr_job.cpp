#include <Poco/SynchronizedObject.h>
#include "../../include/stream_asr_job.h"

namespace poetics::streaming::asr_job {
    StreamAsrJob::StreamAsrJob(/* args */)
    {
    }
    
    StreamAsrJob::~StreamAsrJob()
    {
    }

    string StreamAsrJob::getStreamAsrJobID()
    {
        return _streamAsrJobID;
    }

    int StreamAsrJob::getChannelCount()
    {
        return _channelCount;
    }

    string StreamAsrJob::getAudioEncoding()
    {
        return _audioEncoding;
    }

    int StreamAsrJob::getAudioSampleRate()
    {
        return _audioSampleRate;
    }

    bool StreamAsrJob::getEnableDatalogging()
    {
        return _enableDatalogging;
    }

    string StreamAsrJob::getConversationTitle()
    {
        return _conversationTitle;
    }

    vector<string>* StreamAsrJob::getChannels()
    {
        return _channels.get();
    }

    void StreamAsrJob::setListener(std::shared_ptr<StreamAsrJobListener> listener)
    {
        _listener = listener;
    }

    bool StreamAsrJob::connect()
    {
        return _connect();
    }

    bool StreamAsrJob::disconnect()
    {
        return _disconnect();
    }

    bool StreamAsrJob::isConnected()
    {
        return _isConnected();
    }

    bool StreamAsrJob::enqueueAudioData(int channelIndex, int audioFragmentIndex, const char* data, int dataSize)
    {
        return _enqueueAudioData(channelIndex, audioFragmentIndex, data, dataSize);
    }

    void StreamAsrJob::setLastAudioFragmentIndex(int channelIndex, int audioFragmentIndex)
    {
        _setLastAudioFragmentIndex(channelIndex, audioFragmentIndex);
    }
    void StreamAsrJob::finishEnqueuingAudioData()
    {
        _finishEnqueuingAudioData();
    }

    void StreamAsrJob::onErrorReceived(const string& error)
    {
        _listener->OnErrorReceived(error);
    }

    void StreamAsrJob::onMessageReceived(const string& message)
    {
        if (message.length() == 0) {
            return;
        }

        _listener->OnMessageReceived(message);
    }

    void StreamAsrJob::onDebugMessageReceived(const string& message)
    {
        _listener->OnDebugMessageReceived(message);
    }
}
