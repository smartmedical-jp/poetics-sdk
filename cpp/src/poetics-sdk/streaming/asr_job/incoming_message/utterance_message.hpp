#ifndef UTTERANCE_MESSAGE_H
#define UTTERANCE_MESSAGE_H

#include <string>
using std::string;

namespace poetics::streaming::asr_job::incoming_message {
    class UtteranceMessageBody {
    public:
        UtteranceMessageBody(int channelIndex, int utteranceID, string startAt, string endAt, string text) : 
            _channelIndex(channelIndex), _utteranceID(utteranceID), _startAt(startAt), _endAt(endAt), _text(text) {}

        int getChannelIndex() { return _channelIndex; }
        int getUtteranceID() { return _utteranceID; }
        string getStartAt() { return _startAt; }
        string getEndAt() { return _endAt; }
        string getText() { return _text; }
    private:
        int _channelIndex;
        int _utteranceID;
        string _startAt; // Should be replaced to <chrono>?
        string _endAt; // Should be replaced to <chrono>?
        string _text;
    };

    class UtteranceMessage {
    public:
        UtteranceMessage(string message, UtteranceMessageBody body) : _message(message), _body(body) {}

        string getMessage() { return _message; }
        UtteranceMessageBody getBody() { return _body; }
    private:
        string _message;
        UtteranceMessageBody _body;
    };
}

#endif