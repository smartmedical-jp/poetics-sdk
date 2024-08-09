#ifndef TEMPORARY_UTTERANCE_MESSAGE_h
#define TEMPORARY_UTTERANCE_MESSAGE_h

#include <string>
using std::string;

namespace poetics::streaming::asr_job::incoming_message {
    class TemporaryUtteranceMessageBody;

    class TemporaryUtteranceMessage {
    public:
        TemporaryUtteranceMessage(string message, TemporaryUtteranceMessageBody body) : _message(message), _body(body) {}

        string getMessage() { return _message; }
        TemporaryUtteranceMessageBody getBody() { return _body; }
    private:
        string _message;
        TemporaryUtteranceMessageBody _body;
    };

    class TemporaryUtteranceMessageBody {
    public:
        TemporaryUtteranceMessageBody(int channelIndex, string utteranceID, string startAt, string endAt, string text) : 
            _channelIndex(channelIndex), _utteranceID(utteranceID), _startAt(startAt), _endAt(endAt), _text(text) {}
        
        int getChannelIndex() { return _channelIndex; }
        string getUtteranceID() { return _utteranceID; }
        string getStartAt() { return _startAt; }
        string getEndAt() { return _endAt; }
        string getText() { return _text; }
    private:
        int _channelIndex;
        string _utteranceID;
        string _startAt;
        string _endAt;
        string _text;
    };
}

#endif