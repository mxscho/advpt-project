# AdvPT Project - StarCraft II Build Order Simulator

## How to Build

### Prerequisites

* Make sure you have [CMake](https://cmake.org/) (version >= 2.8) installed.
* If you're using Windows, make sure the directory where `cmake.exe` is located at is part of your environment's `PATH` variable.

### Linux

Decide which compiler you want to use and replace  `{COMPILER}` following further down with the respective value.

* `gcc` (GCC/g++)

Generate Makefile and automatically start build:

```text
$ ./build_linux_{COMPILER}.sh
```

### Windows (Visual Studio)

Decide which Visual Studio and compiler you want to use and replace  `{COMPILER}` following further down with the respective value.

* `vs2012` (Visual Studio 2012/Visual C++)
* `vs2013` (Visual Studio 2013/Visual C++)
* `vs2015` (Visual Studio 2015/Visual C++)

Generate Visual Studio project and solution files:

```text
$ .\generate_windows_{COMPILER}.bat
```

Open solution file `.\bin\Project.sln` in Visual Studio and build the project. Make sure to select `sc2simulator` as the startup project.

## How to Use

### Linux

Run with the following command:

```text
$ ./bin/sc2simulator
```

### Windows

Run:

```text
$ .\bin\{Debug|Release}\sc2simulator.exe
```
