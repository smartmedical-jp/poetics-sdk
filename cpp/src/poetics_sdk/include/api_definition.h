#ifndef API_DEFINITION_H
#define API_DEFINITION_H

#if defined(_WIN32)
    #if defined(poetics_sdk_EXPORTS) || defined(_USRDLL)
        #define POETICS_API _declspec(dllexport)
    #else
        #define POETICS_API __declspec(dllimport)
    #endif
    #define POETICS_API_CALL __stdcall
#else
    #define POETICS_API
    #define POETICS_API_CALL
#endif

#endif
