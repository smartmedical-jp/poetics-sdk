# Poetics SDK (C++)

[日本語はこちら](./README.ja.md)

- [Poetics SDK (C++)](#poetics-sdk-c)
- [1. Description](#1-description)
- [2. Key features](#2-key-features)
- [3. Build Guide](#3-build-guide)
- [4. SDK documentation](#4-sdk-documentation)

# 1. Description

This repository contains a `C++` based Poetics SDK.

Offically supports `Windows` on `x64` platforms, but potentially other OS can also build the SDK.

# 2. Key features

Via WebSocket stream, you can send audio data to the Poetics Speech API server and receive the Speech-to-text result in real time.

# 3. Build Guide

Go to [BUILD.md](docs/en-US/BUILD.md).

After building the SDK, you can use this library in your own C++ project with following steps:

1. Add `include` folder to your project's additional include directory
2. Add folder that contains the `poetics_sdk.lib` file to your project's additional linker directory
3. Add `poetics_sdk.lib` to the input (if you are using Visual Studio)
4. Copy the `poetics_sdk.dll` to the build directory where your application is located

Note that you should separate the Debug/Release build of this SDK to different directory.

# 4. SDK documentation

For the API provided in this SDK library, check [API.md](docs/en-US/API.md).

For details on the Speech Server API, go to [[poetics-api.readme.io](https://poetics-api.readme.io/docs/streaming-asr)].
