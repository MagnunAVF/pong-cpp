#include <SDL.h>
#include <cstdio>

namespace {
constexpr int kWindowWidth = 800;
constexpr int kWindowHeight = 600;
}  // namespace

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        kWindowWidth, kWindowHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
