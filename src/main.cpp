#include "Config.hpp"
#include "Options.hpp"
#include "World.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace intergrid;

static bool is_quit_event(SDL_Event& event)
{
    return event.type == SDL_QUIT
        || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_q);
}

static void simulate(Options& opts, SDL_Renderer* renderer)
{
    SDL_Event event;
    SDL_Rect screen_dims;
    if (opts.draw) {
        SDL_RenderGetViewport(renderer, &screen_dims);
    } else {
        screen_dims.w = opts.screen_width;
        screen_dims.h = opts.screen_height;
    }
    Config& conf = opts.conf;
    unsigned long sim_tick = 0;
    size_t world_width = conf.world_width > 0.
        ? conf.world_width
        : screen_dims.w / opts.pixel_width;
    size_t world_height = conf.world_height > 0.
        ? conf.world_height
        : screen_dims.h / opts.pixel_height;
    World world(world_width, world_height);
    srand(time(NULL));
    world.randomize(conf);
    for (;;) {
        Uint32 ticks = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (is_quit_event(event)) {
                return;
            }
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
}

int main(int argc, char* argv[])
{
    int status = EXIT_FAILURE;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    Options opts(argc, argv);
	Uint32 init_flags = SDL_INIT_TIMER | SDL_INIT_EVENTS;
	if (opts.draw) {
		init_flags |= SDL_INIT_VIDEO;
	}
    if (SDL_Init(init_flags)) {
        fprintf(stderr, "SDL initialization failed; %s\n", SDL_GetError());
        goto error_sdl_init;
    }
    if (opts.draw) {
        window = SDL_CreateWindow("Intergrid", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, opts.screen_width, opts.screen_height,
            SDL_WINDOW_SHOWN);
        if (!window) {
            fprintf(stderr, "SDL window creation failed; %s\n", SDL_GetError());
            goto error_create_window;
        }
        if (opts.fullscreen
            && SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP)
                < 0) {
            fprintf(stderr, "SDL window fullscreenification failed; %s\n",
                SDL_GetError());
            // Keep executing.
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (!renderer) {
            fprintf(
                stderr, "SDL renderer creation failed; %s\n", SDL_GetError());
            goto error_create_surface;
        }
        // Just in case the background is another color:
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }
    simulate(opts, renderer);
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
