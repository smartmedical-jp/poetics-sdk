#ifndef UTTERANCE_H
#define UTTERANCE_H

#include <string>
using std::string;

namespace poetics::streaming::asr_job::core {
    class Utterance {
    public:
        Utterance(string utteranceId, bool isTemporary, int channelIndex, string startAt, string endAt, string text) {
            _utteranceId = utteranceId;
            _isTemporary = isTemporary;
            _channelIndex = channelIndex;
            _startAt = startAt;
            _endAt = endAt;
            _text = text;
        }

        string getUtteranceId() { return _utteranceId; }
        bool getIsTemporary() { return _isTemporary; }
        int getChannelIndex() { return _channelIndex; }
        string getStartAt() { return _startAt; }
        string getEndAt() { return _endAt; }
        string getText() { return _text; }
    private:
        string _utteranceId;
        bool _isTemporary;
        int _channelIndex;
        string _startAt; // Should be replaced to <chrono>?
        string _endAt; // Should be replaced to <chrono>?
        string _text;
    };
}

#endif