# Poetics SDK (C++)

- [Poetics SDK (C++)](#poetics-sdk-c)
- [1. Description](#1-description)
- [2. Key features](#2-key-features)
- [3. Reserved](#3-reserved)
- [4. Setup Guide](#4-setup-guide)
  - [4.1 Windows-specific setup guide](#41-windows-specific-setup-guide)
    - [4.1.1 Install Build Tools for Visual Studio 2022](#411-install-build-tools-for-visual-studio-2022)
    - [4.1.2 Install CMake \& Ninja](#412-install-cmake--ninja)
    - [4.1.3 Open VSCode from Developer terminal for Visual Studio](#413-open-vscode-from-developer-terminal-for-visual-studio)
  - [5.2 Setting up VSCode Extensions](#52-setting-up-vscode-extensions)
- [6. Build, Debug, and Test](#6-build-debug-and-test)
  - [6.1 How to build](#61-how-to-build)
  - [6.2 How to debug](#62-how-to-debug)
  - [6.3 How to run test](#63-how-to-run-test)
- [7. Integrated Development Environment (IDE) Support](#7-integrated-development-environment-ide-support)

# 1. Description

This repository contains a `C++` based Poetics SDK that leverages [vcpkg](https://github.com/microsoft/vcpkg) and [CMake](https://www.cmake.org)'s [CMakePresets.json](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html) to build and test the SDK code.

Supports `Linux`/`macOS`/`Windows` on `x64` platforms, currently tested on Windows only.

# 2. Key features

To be written

# 3. Reserved

To be written

# 4. Setup Guide

## 4.1 Windows-specific setup guide

If you are developing in Linux or macOS, skip to [Section 5.2](#52-setting-up-vscode-extensions).

### 4.1.1 Install Build Tools for Visual Studio 2022

If you have already installed Visual Studio 2022, you can skip this part.

- Go to https://visualstudio.microsoft.com/downloads/
- In `All Downloads`, open up `Tools for Visual Studio` and download `Build Tools for Visual Studio 2022`
- Install the downloaded file

### 4.1.2 Install CMake & Ninja

- Install CMake (with adding PATH) [[Download](https://cmake.org/download/)]
- Install Ninja build tool using winget:
  - `winget install Ninja-build.Ninja`

### 4.1.3 Open VSCode from Developer terminal for Visual Studio

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

In this case, you **should remove the `builds` directory** and run the VSCode by launching the `launch_devtools_x**.bat` mentioned above.

## 5.2 Setting up VSCode Extensions

Go to extensions tab and install following extensions:

- C/C++
- C/C++ Extension Pack
- CMake
- CMake Langugage Support
- CMake Tools

After you install the above extensions and select in the status bar the CMake preset (e.g. `ninja-multi-vcpkg`), as show in the following image:

![CMake's preset selection in the status bar of Visual Studio Code](./img/vscode_cmakepresets_selection.png)

# 6. Build, Debug, and Test

Before build or debug, click the following button to choose the target:

![CMake's project selection in the status bar of Visual Studio Code](./img/vscode_target_selection.png)

Then choose the target:

![CMake's project selection in the status bar of Visual Studio Code (2)](./img/vscode_target_selection_2.png)

## 6.1 How to build

- Open the command palette(`ctrl+shift+p`)
- Run `CMake: Build` to build all targets
- Run `CMake: Build target` to build the target currently selected

## 6.2 How to debug

- Open the command palette(`ctrl+shift+p`)
- Run `CMake: Debug`

## 6.3 How to run test

- Open the command palette(`ctrl+shift+p`)
- Run `CMake: Run Tests`

# 7. Integrated Development Environment (IDE) Support

The major `C++` IDEs like Visual Studio should already support `CMakePresets.json` and require no particular configuration.

For example [Visual Studio Code](https://code.visualstudio.com/) with the [CMake Tools extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) let you to open the root folder of this repository, and select in the status bar the CMake preset (e.g. `ninja-multi-vcpkg`), as show in the following image:

![CMake's preset selection in the status bar of Visual Studio Code](./img/vscode_cmakepresets_selection.png)

<br>
