// main.cpp
#include <iostream>
#include <string>
using std::string;
#include <fmt/core.h>

#include "poetics/poetics-sdk.h"

// main function!
int main()
{
    string path = "C:\\REPO\\poetics-sdk-cpp\\lib\\x64\\Debug\\poetics-sdk.dll";
    auto sdk = new Poetics::SpeechAPI(path);
    fmt::print("The Hello() returned {}\n", sdk->Hello());
    
    delete sdk;
    sdk = nullptr;

    return 0;
}