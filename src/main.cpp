#include "Config.hpp"
#include "World.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int status = EXIT_FAILURE;
    unsigned long sim_tick;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    Config conf;
    if (conf.parse("conf") < 0) {
        fputs("Unable to parse configuration\n", stderr);
        exit(EXIT_FAILURE);
    }
    size_t world_width = conf.world_width > 0. ? conf.world_width : 1;
    size_t world_height = conf.world_height > 0. ? conf.world_height : 1;
    World world(world_width, world_height);
    srand(time(NULL));
    world.randomize(conf);
    if (SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL initialization failed; %s\n", SDL_GetError());
        goto error_sdl_init;
    }
    window = SDL_CreateWindow("Intergrid", SDL_WINDOWPOS_CENTERED,
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
    sim_tick = 0;
    for (;;) {
        Uint32 ticks = SDL_GetTicks();
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
            break;
        }
        world.draw(renderer);
        SDL_RenderPresent(renderer);
        world.simulate(conf);
        ++sim_tick;
        printf("%lu %f %f %f %f %ld %f\n", sim_tick,
            world.count_total_temperature(), world.count_total_plants(),
            world.count_total_water(), world.count_total_clouds(),
            world.count_total_herbivores(), world.count_total_herbivore_food());
        fflush(stdout);
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
    exit(status);
}
