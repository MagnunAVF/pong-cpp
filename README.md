# Pong Cpp

A classic two-paddle Pong game in C++17 using SDL2.

## Prerequisites

- CMake 3.16+
- A C++17 compiler (e.g. AppleClang/Clang, GCC)
- SDL2
- SDL2_ttf

On macOS, install SDL2 and SDL2_ttf via Homebrew:

```sh
brew install sdl2 sdl2_ttf
```

On-screen text (control hints, game-over message) is rendered with the
system Monaco font (`/System/Library/Fonts/Monaco.ttf`), so this currently
assumes macOS.

## Build

```sh
cmake -S . -B build
cmake --build build
```

## Run

```sh
./build/pong
```
