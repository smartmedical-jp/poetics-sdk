// main.cpp
#include <iostream>
#include <string>
using std::string;
#include <fmt/core.h>

#include "poetics/poetics-sdk.h"

// main function!
int main(int argc, char* argv[])
{
    string path;
    if (argc == 2) {
        path = argv[1];
    } else if (argc == 1) {
        path = "poetics-sdk.dll";
    }
    auto sdk = new Poetics::SpeechAPI(path);
    if (!sdk->isLibraryLoaded()) {
        fmt::print("Failed to load library\n");
        return -1;
    }
    
    fmt::print("The Hello() returned {}\n", sdk->Hello());
    fmt::print("TestAudioBuffer returned {}\n", sdk->TestAudioBuffer());
    fmt::print("TestOutgoingMessage returned {}\n", sdk->TestOutgoingMessage());
    fmt::print("TestIncomingMessage returned {}\n", sdk->TestIncomingMessage());
    
    delete sdk;
    sdk = nullptr;

    return 0;
}