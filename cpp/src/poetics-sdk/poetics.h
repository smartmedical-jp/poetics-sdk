#ifndef POETICS_H
#define POETICS_H

#include <random>
#include <chrono>

#include <fmt/core.h>

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

#endif