#include "Config.hpp"
#include "Options.hpp"
#include "World.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace intergrid;

int main(int argc, char* argv[])
{
    int status = EXIT_FAILURE;
    unsigned long sim_tick;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    Options opts(argc, argv);
    Config& conf = opts.conf;
    size_t world_width
        = conf.world_width > 0. ? conf.world_width : opts.screen_width / 10;
    size_t world_height
        = conf.world_height > 0. ? conf.world_height : opts.screen_height / 10;
    World world(world_width, world_height);
    srand(time(NULL));
    world.randomize(conf);
    if (SDL_Init(opts.draw ? SDL_INIT_VIDEO : 0)) {
        printf("SDL initialization failed; %s\n", SDL_GetError());
        goto error_sdl_init;
    }
    if (opts.draw) {
        window = SDL_CreateWindow("Intergrid", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, opts.screen_width, opts.screen_height,
            SDL_WINDOW_SHOWN);
        if (!window) {
            printf("SDL window creation failed; %s\n", SDL_GetError());
            goto error_create_window;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (!renderer) {
            printf("SDL renderer creation failed; %s\n", SDL_GetError());
            goto error_create_surface;
        }
    }
    sim_tick = 0;
    for (;;) {
        Uint32 ticks = SDL_GetTicks();
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
            break;
        }
        if (opts.draw) {
            world.draw(renderer);
            SDL_RenderPresent(renderer);
        }
        world.simulate(conf);
        ++sim_tick;
        if (opts.print_stats) {
            printf("%lu %f %f %f %f %ld %f\n", sim_tick,
                world.count_total_temperature(), world.count_total_plants(),
                world.count_total_water(), world.count_total_clouds(),
                world.count_total_herbivores(),
                world.count_total_herbivore_food());
            fflush(stdout);
        }
        Uint32 new_ticks = SDL_GetTicks();
        if (new_ticks - ticks < opts.frame_delay) {
            SDL_Delay(opts.frame_delay - (new_ticks - ticks));
        }
    }
    status = EXIT_SUCCESS;
    if (opts.draw) {
        SDL_DestroyRenderer(renderer);
    }
error_create_surface:
    if (opts.draw) {
        SDL_DestroyWindow(window);
    }
error_create_window:
    SDL_Quit();
error_sdl_init:
    exit(status);
}
