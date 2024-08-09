#define POETICS_H_EXPORTS
#include "poetics.h"

int hello()
{
    std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
    return generator();
}