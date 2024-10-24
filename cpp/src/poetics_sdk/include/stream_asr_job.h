#ifndef STREAM_ASR_JOB_H
#define STREAM_ASR_JOB_H

#include <string>
using std::string;
#include <memory>
using std::shared_ptr;
using std::unique_ptr;
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;

#include "stream_asr_job_listener.h"

namespace poetics::streaming::asr_job {
    class StreamAsrJob
    {
    private:
        shared_ptr<StreamAsrJobListener> _listener;
    public:
        virtual ~StreamAsrJob();

        string getStreamAsrJobID();
        int getChannelCount();
        string getAudioEncoding();
        int getAudioSampleRate();
        bool getEnableDatalogging();
        string getConversationTitle();
        vector<string>* getChannels();

        void setListener(shared_ptr<StreamAsrJobListener> listener);
        
        bool connect();
        bool disconnect();
        bool isConnected();
        bool enqueueAudioData(int channelIndex, int audioFragmentIndex, const char* data, int dataSize);
        void setLastAudioFragmentIndex(int channelIndex, int audioFragmentIndex);
        void finishEnqueuingAudioData();
    protected:
        string _endpoint;
        string _apiKey;
    
        string _streamAsrJobID;
        string _audioEncoding;
        int _audioSampleRate;
        int _channelCount;
        bool _enableDatalogging;
        string _conversationTitle;
        unique_ptr<vector<string>> _channels;

        StreamAsrJob(/* args */);

        virtual bool _connect() = 0;
        virtual bool _disconnect() = 0;
        virtual bool _isConnected() = 0;

        virtual void _sendMessage(const char* data, int dataSize) = 0;
        virtual bool _enqueueAudioData(int channelIndex, int audioFragmentIndex, const char* data, int dataSize) = 0;
        virtual void _setLastAudioFragmentIndex(int channelIndex, int audioFragmentIndex) = 0;
        virtual void _finishEnqueuingAudioData() = 0;

        void onErrorReceived(const string& error);
        void onMessageReceived(const string& message);
        void onDebugMessageReceived(const string& message);
    };
}
#endif