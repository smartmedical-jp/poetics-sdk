#ifndef POETICS_H
#define POETICS_H

#include <random>
#include <chrono>
#include <iostream>
using std::cout;
using std::endl;

#include <fmt/core.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "streaming/streaming_client.h"

#ifdef _WIN32
#ifdef POETICS_H_EXPORTS
#define POETICS_H_API __declspec(dllexport)
#else
#define POETICS_H_API __declspec(dllimport)
#endif

#elif __APPLE__
#define POETICS_H_API

#elif __linux__
#define POETICS_H_API
#endif

extern "C" POETICS_H_API int hello();
extern "C" POETICS_H_API int test_audio_buffer();
extern "C" POETICS_H_API int test_outgoing_message();

#endif