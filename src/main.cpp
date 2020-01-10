#include "Config.hpp"
#include "World.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int status = EXIT_FAILURE;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    Config conf;
    World world(80, 60);
    if (conf.parse("conf") < 0) {
        fputs("Unable to parse configuration\n", stderr);
        goto error_config;
    }
    srand(time(NULL));
    world.randomize(conf);
    if (SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL initialization failed; %s\n", SDL_GetError());
        goto error_sdl_init;
    }
    window = SDL_CreateWindow("Grid", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL window creation failed; %s\n", SDL_GetError());
        goto error_create_window;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        printf("SDL renderer creation failed; %s\n", SDL_GetError());
        goto error_create_surface;
    }
    for (;;) {
        Uint32 ticks = SDL_GetTicks();
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
            break;
        }
        world.draw(renderer);
        SDL_RenderPresent(renderer);
        world.simulate(conf);
        Uint32 new_ticks = SDL_GetTicks();
        if (new_ticks - ticks < 100) {
            SDL_Delay(100 - (new_ticks - ticks));
        }
    }
    status = EXIT_SUCCESS;
    SDL_DestroyRenderer(renderer);
error_create_surface:
    SDL_DestroyWindow(window);
error_create_window:
    SDL_Quit();
error_sdl_init:
error_config:
    exit(status);
}
