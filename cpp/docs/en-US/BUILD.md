# Setup Guide

- [Setup Guide](#setup-guide)
  - [1. Setting up build environment](#1-setting-up-build-environment)
    - [1.1 Install Build Tools for Visual Studio 2022](#11-install-build-tools-for-visual-studio-2022)
    - [1.2 Install CMake \& Ninja](#12-install-cmake--ninja)
    - [1.3 Open VSCode from Developer terminal for Visual Studio](#13-open-vscode-from-developer-terminal-for-visual-studio)
  - [2. Setting up VSCode Extensions](#2-setting-up-vscode-extensions)
- [3. Build, Debug, and Test](#3-build-debug-and-test)
  - [3.1 How to build](#31-how-to-build)
  - [3.2 How to debug](#32-how-to-debug)
  - [3.3 How to run test](#33-how-to-run-test)
- [4. Integrated Development Environment (IDE) Support](#4-integrated-development-environment-ide-support)


## 1. Setting up build environment

### 1.1 Install Build Tools for Visual Studio 2022

If you have already installed Visual Studio 2022, you can skip this part.

- Go to https://visualstudio.microsoft.com/downloads/
- In `All Downloads`, open up `Tools for Visual Studio` and download `Build Tools for Visual Studio 2022`
- Install the downloaded file

### 1.2 Install CMake & Ninja

- Install CMake (with adding PATH) [[Download](https://cmake.org/download/)]
- Install Ninja build tool using winget:
  - `winget install Ninja-build.Ninja`

### 1.3 Open VSCode from Developer terminal for Visual Studio

> [!IMPORTANT]
> To properly build the project, you should **always** run VSCode from Developer terminal for Visual Studio.
> Follow the guide below firmly.

- [**IMPORTANT**] If you are to build x86 binary,
  - Run `launch_devtools_x86.bat`
- [**IMPORTANT**] If you are to build x64 binary,
  - Run `launch_devtools_x64.bat`
- Run `code .`

Now you can build and run the project! Keep in mind you should always open the project like this way.

(If you clone this repository and open with plain VSCode, you will see following error after all:

```
[cmake] CMake Error: CMake was unable to find a build program corresponding to "Ninja Multi-Config".  CMAKE_MAKE_PROGRAM is not set.  You probably need to select a different build tool.
[cmake] CMake Error: CMAKE_C_COMPILER not set, after EnableLanguage
[cmake] CMake Error: CMAKE_CXX_COMPILER not set, after EnableLanguage
```

In this case, you **should remove the `builds` directory** and run the VSCode again by launching the `launch_devtools_x**.bat` mentioned above.

## 2. Setting up VSCode Extensions

Go to extensions tab and install following extensions:

- C/C++
- C/C++ Extension Pack
- CMake
- CMake Langugage Support
- CMake Tools

After you install the above extensions and select in the status bar the CMake configuration (e.g. `Ninja Multi-Config`), as show in the following image:

![CMake's preset selection in the status bar of Visual Studio Code](../img/vscode_cmakepresets_selection.png)

# 3. Build, Debug, and Test

Currently there are two build configures:

* Ninja Multi-Config: Common build configure (use in Debug mode)
* Ninja Win64 (Static): Windows-specific build to generate **single** library file (please **use in Release mode**).

The former config will build the libraries dependent to this SDK separately like below:

![Build result of Ninja Multi-Config](../img/ninja-multi-config.png)

The latter config will merge the libraries dependent to this SDK (statically) like below:

![Build result of Ninja Win64-Static](../img/ninja-win64-static.png)

## 3.1 How to build

- Open the command palette(`ctrl+shift+p`)
- Run `CMake: Build` to build all targets
- Run `CMake: Build target` to build the target currently selected

## 3.2 How to debug

- Open the command palette(`ctrl+shift+p`)
- Run `CMake: Debug`

## 3.3 How to run test

- Open the command palette(`ctrl+shift+p`)
- Run `CMake: Run Tests`

# 4. Integrated Development Environment (IDE) Support

The major `C++` IDEs like Visual Studio should already support `CMakePresets.json` and require no particular configuration.

For example [Visual Studio Code](https://code.visualstudio.com/) with the [CMake Tools extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) let you to open the root folder (`cpp`) of this repository, and select the default build target (ex: `all, poetics-sdk, poetics-standalone, ...`) or target to launch (ex: `poetics_standalone, poetics_test`), as shown in the following image:

![CMake's preset selection in the status bar of Visual Studio Code](../img/vscode_cmakepresets_selection.png)
