## Overview
The project appears to be a C program that visualizes quad trees using a graphical user interface (GUI). The main functionality involves rendering and interacting with quad trees, likely for educational or benchmarking purposes. The GUI is built on top of a custom library, possibly written in C as well.

## Features
- Quad tree visualization
- Basic GUI interaction (panning, zooming)
- Debug mode for profiling performance

## Project Structure
```
Gui_Quad_Trees/
├── build/              # .exe files produced by Main.c
├── libs/               # *.c for bin
├── lib/                # librarys for my own languages
├── code/               # scripts from my custom languages for example .rex, .ll, .omml
├── data/               # Datafile for example .txt or dumped files
├── assets/             # images and sound files
├── src/                # source code
│   ├── Main.c          # Entry point
│   └── *.h             # stand alone Header-based C-files, without *.c files that implement it
├── Makefile.linux      # Linux Build configuration
├── Makefile.windows    # Windows Build configuration
├── Makefile.wine       # Wine Build configuration for cross-compiling to Windows
└── Makefile.web        # Emscripten Build configuration for compiling to WebAssembly
```

## Prerequisites
- C/C++ Compiler and Debugger (GCC, Clang)
- Make utility
- Standard development tools
- Libraries needed in specific projects:
  - X11 for Linux GUI
  - user32, gdi32, winmm for Windows GUI
  - SDL2 for WebAssembly (if using Emscripten)

## Build & Run
### Linux
To build and run on Linux:
```sh
cd Gui_Quad_Trees
make -f Makefile.linux all
./build/Main
```
For a clean rebuild:
```sh
make -f Makefile.linux clean
make -f Makefile.linux all
```

### Windows
To build and run on Windows (using MinGW-w64):
```sh
cd Gui_Quad_Trees
make -f Makefile.windows all
build\Main.exe
```
For a clean rebuild:
```sh
make -f Makefile.windows clean
make -f Makefile.windows all
```

### Wine
To build and run on Linux for Windows using Wine:
```sh
cd Gui_Quad_Trees
make -f Makefile.wine all
WINEPREFIX=~/wine64 WINEARCH=win64 wine build\Main.exe
```
For a clean rebuild:
```sh
make -f Makefile.wine clean
make -f Makefile.wine all
```

### WebAssembly (Emscripten)
To build and run on the web using Emscripten:
```sh
cd Gui_Quad_Trees
make -f Makefile.web all
emrun --no_browser --port 8080 build/index.html
```
For a clean rebuild:
```sh
make -f Makefile.web clean
make -f Makefile.web all
```