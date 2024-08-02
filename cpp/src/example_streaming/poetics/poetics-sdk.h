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

    class SpeechAPI
    {
    public:
        fHello Hello;

        SpeechAPI(string libraryPath)
        {   // "C:\\REPO\\poetics-sdk-cpp\\lib\\x64\\Debug\\poetics-sdk.dll"
            init(libraryPath);
        }

        ~SpeechAPI()
        {
            FreeLibrary(_hDLL);
        }
    private:
        HMODULE _hDLL;
        string _libraryPath = "";
        
        SpeechAPI() {};
        bool init(string libraryPath)
        {
            bool result = false;
            
            _hDLL = LoadLibraryA(libraryPath.c_str());
            if (_hDLL != NULL)
            {
                Hello = (fHello)GetProcAddress(_hDLL, "hello");

                _libraryPath = libraryPath;
                result = true;
            }
            return result;
        }
    };
}

#endif
