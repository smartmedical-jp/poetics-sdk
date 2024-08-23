#ifndef POETICS_H
#define POETICS_H

#include <string>
using std::string;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <fmt/core.h>

namespace Poetics
{
    typedef int (*fHello)(void);
    typedef int (*fTestAudioBuffer)(void);
    typedef int (*fTestOutgoingMessage)(void);
    typedef int (*fTestIncomingMessage)(void);

    class SpeechAPI
    {
    public:
        fHello Hello;
        fTestAudioBuffer TestAudioBuffer;
        fTestOutgoingMessage TestOutgoingMessage;
        fTestIncomingMessage TestIncomingMessage;
        bool isLibraryLoaded() { return _libraryLoaded; }

        SpeechAPI(string libraryPath)
        {   // "C:\\REPO\\poetics-sdk-cpp\\lib\\x64\\Debug\\poetics-sdk.dll"
            _libraryLoaded = init(libraryPath);
        }

        ~SpeechAPI()
        {
            FreeLibrary(_hDLL);
        }
    private:
        HMODULE _hDLL;
        string _libraryPath = "";
        bool _libraryLoaded = false;
        
        SpeechAPI() {};
        bool init(string libraryPath)
        {
            bool result = false;
            
            _libraryPath = libraryPath;
            _hDLL = LoadLibraryA(libraryPath.c_str());
            if (_hDLL != NULL)
            {
                Hello = (fHello)GetProcAddress(_hDLL, "hello");
                TestAudioBuffer = (fTestAudioBuffer)GetProcAddress(_hDLL, "test_audio_buffer");
                TestOutgoingMessage = (fTestOutgoingMessage)GetProcAddress(_hDLL, "test_outgoing_message");
                TestIncomingMessage = (fTestIncomingMessage)GetProcAddress(_hDLL, "test_incoming_message");

                result = testInterfaces();
            }
            return result;
        }

        bool testInterfaces()
        {
            if (Hello == NULL || TestAudioBuffer == NULL || 
            TestOutgoingMessage == NULL || TestIncomingMessage == NULL)
            {
                return false;
            }
            return true;
        }
    };
}

#endif
