#ifndef STREAM_ASR_JOB_H
#define STREAM_ASR_JOB_H
#include "api_definition.h"

#include <string>
using std::string;
#include <memory>
using std::shared_ptr;
using std::unique_ptr;
#include <vector>
using std::vector;

#include "stream_asr_job_listener.h"

namespace poetics {
    namespace streaming {
        namespace asr_job {
            class StreamAsrJob
            {
            public:
                POETICS_API virtual ~StreamAsrJob();

                POETICS_API string getStreamAsrJobID();
                POETICS_API int getChannelCount();
                POETICS_API string getAudioEncoding();
                POETICS_API int getAudioSampleRate();
                POETICS_API bool getEnableDatalogging();
                POETICS_API string getConversationTitle();
                POETICS_API vector<string>* getChannels();

                POETICS_API void setListener(shared_ptr<StreamAsrJobListener> listener);
        
                POETICS_API bool connect();
                POETICS_API bool disconnect();
                POETICS_API bool isConnected();
                POETICS_API bool enqueueAudioData(int channelIndex, int audioFragmentIndex, const char* data, int dataSize);
                POETICS_API void setLastAudioFragmentIndex(int channelIndex, int audioFragmentIndex);
                POETICS_API void finishEnqueuingAudioData();
            private:
                shared_ptr<StreamAsrJobListener> _listener;
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
    }
}
#endif
