#ifndef MAIN_H
#define MAIN_H
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <string>
using std::string;
#include <memory>
using std::unique_ptr;
#include <vector>
using std::vector;
#include <fstream>
using std::ifstream;
#include <chrono>
#include <thread>

#include <cstdlib>

#include "streaming_client.h"

#ifdef WIN32
#define POETICS_SETENV _putenv_s
#else
#define POETICS_SETENV setenv
#endif

#endif