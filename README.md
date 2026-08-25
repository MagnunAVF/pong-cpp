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

## Versioning & releases

The current version lives in the `VERSION` file (plain [semver](https://semver.org/),
e.g. `0.1.0`) and is baked into the build (CMake project version, window
title) at configure time.

To bump it:

```sh
./scripts/bump-version.sh {major|minor|patch}
```

This updates `VERSION`, commits it, and creates an annotated `vX.Y.Z` git
tag locally. It refuses to run with a dirty working tree. Push the result
yourself when ready:

```sh
git push && git push origin vX.Y.Z
```

CI (`.github/workflows/release.yml`) builds Linux, macOS, and Windows
binaries whenever code actually lands (merges) on `dev` or `main`:

- Merging into `dev` publishes a pre-release build tagged
  `vX.Y.Z-rc.<run number>`.
- Merging into `main` publishes a full release tagged `vX.Y.Z`, using
  whatever `VERSION` currently contains — bump it first so the release
  tag is what you intend.
