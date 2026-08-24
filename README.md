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

On-screen text (control hints, game-over message) is rendered with a
system font, found automatically at startup from a list of common paths
on macOS, Linux, and Windows. If none of those paths exist, the game
still runs, just without that text.

## Build

```sh
cmake -S . -B build
cmake --build build
```

## Run

```sh
./build/pong
```
