#include "World.hpp"

World::World(size_t width, size_t height)
    : temperature(width, height)
    , plants(width, height)
    , water(width, height)
    , clouds(width, height)
    , herbivores(width, height)
{
}

size_t World::get_width() { return temperature.get_width(); }

size_t World::get_height() { return temperature.get_height(); }

static float random_float() { return (float)rand() / RAND_MAX; }

void World::randomize()
{
    for (size_t x = 0; x < get_width(); ++x) {
        for (size_t y = 0; y < get_height(); ++y) {
            plants.at(x, y) = random_float() * 10.;
            temperature.at(x, y) = random_float() * 10.;
            water.at(x, y) = random_float() * 10.;
            clouds.at(x, y) = random_float() * 10.;
            herbivores.at(x, y).food
                = rand() / (RAND_MAX / 100) == 0 ? 10. : 0.;
        }
    }
}

static float flow(float a, float b, float portion) { return (b - a) * portion; }

static void disperse(Grid<float>& grid, float portion, Grid<float>& next)
{
    for (size_t x = 0; x < grid.get_width(); ++x) {
        for (size_t y = 0; y < grid.get_height(); ++y) {
            float& here = next.at(x, y);
            float& right = next.at_small_trans(x, y, 1, 0);
            float& below = next.at_small_trans(x, y, 0, 1);
            here = grid.at(x, y);
            right = grid.at_small_trans(x, y, 1, 0);
            below = grid.at_small_trans(x, y, 0, 1);
            float flow_right = flow(here, right, portion);
            float flow_below = flow(here, below, portion);
            here += flow_right;
            right -= flow_right;
            here += flow_below;
            below -= flow_below;
            grid.at(x, y) = here;
        }
    }
}

static void breed_plants(Grid<float>& plants, Grid<float>& temperature,
    Grid<float>& water, Grid<float>& clouds)
{
    for (size_t x = 0; x < plants.get_width(); ++x) {
        for (size_t y = 0; y < plants.get_height(); ++y) {
            float& here = plants.at(x, y);
            float mul = 1.;
            if (water.at(x, y) < here) {
                mul *= 0.93;
            }
            if (temperature.at(x, y) > here) {
                mul *= 1.06;
            } else {
                mul *= 0.99;
            }
            if (plants.at_small_trans(x, y, 1, 0) > here
                && plants.at_small_trans(x, y, 0, 1) > here
                && plants.at_small_trans(x, y, -1, 0) > here
                && plants.at_small_trans(x, y, 0, -1) > here) {
                mul *= 0.9;
            }
            mul -= clouds.at(x, y) / 1000.;
            here *= mul;
        }
    }
}

static void vaporize(Grid<float>& temperature, Grid<float>& plants,
    Grid<float>& water, Grid<float>& clouds)
{
    for (size_t x = 0; x < temperature.get_width(); ++x) {
        for (size_t y = 0; y < temperature.get_height(); ++y) {
            float vapor = temperature.at(x, y) / 100. + plants.at(x, y) / 100.;
            if (water.at(x, y) < vapor) {
                vapor = water.at(x, y);
            }
            water.at(x, y) -= vapor;
            clouds.at(x, y) += vapor;
        }
    }
}

static void precipitate(Grid<float>& clouds, Grid<float>& water)
{
    for (size_t x = 0; x < clouds.get_width(); ++x) {
        for (size_t y = 0; y < clouds.get_height(); ++y) {
            if (clouds.at(x, y) > 6.) {
                clouds.at(x, y) -= 0.1;
                water.at(x, y) += 0.1;
            }
        }
    }
}

static void cool_down(Grid<float>& temperature)
{
    for (size_t x = 0; x < temperature.get_width(); ++x) {
        for (size_t y = 0; y < temperature.get_height(); ++y) {
            temperature.at(x, y) *= 0.99;
        }
    }
}

static void produce_body_heat(Grid<float>& plants, Grid<float>& temperature)
{
    for (size_t x = 0; x < plants.get_width(); ++x) {
        for (size_t y = 0; y < plants.get_height(); ++y) {
            temperature.at(x, y) += plants.at(x, y) / 50.;
        }
    }
}

static void eat_plants(Grid<Herbivore>& herbivores, Grid<float>& plants)
{
    for (size_t x = 0; x < herbivores.get_width(); ++x) {
        for (size_t y = 0; y < herbivores.get_height(); ++y) {
            if (herbivores.at(x, y).food > 0.) {
                herbivores.at(x, y).food += plants.at(x, y) * 0.1 - 0.2;
                plants.at(x, y) = 0;
            }
        }
    }
}

static void remove_move_markers(Grid<Herbivore>& herbivores)
{
    for (size_t x = 0; x < herbivores.get_width(); ++x) {
        for (size_t y = 0; y < herbivores.get_height(); ++y) {
            herbivores.at(x, y).moved = false;
        }
    }
}

static void move_herbivores(Grid<Herbivore>& herbivores, Grid<float>& plants)
{
    for (size_t x = 0; x < herbivores.get_width(); ++x) {
        for (size_t y = 0; y < herbivores.get_height(); ++y) {
            Herbivore& here = herbivores.at(x, y);
            if (here.food > 0. && !here.moved) {
                size_t around[4][2]
                    = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
                int idx = -1;
                float max = 0.;
                for (int i = 0; i < 4; ++i) {
                    size_t there_x = x;
                    size_t there_y = y;
                    herbivores.small_trans(
                        there_x, there_y, around[i][0], around[i][1]);
                    if (herbivores.at(there_x, there_y).food <= 0.
                        && plants.at(there_x, there_y) > max) {
                        idx = i;
                        max = plants.at(there_x, there_y);
                    }
                }
                if (idx >= 0) {
                    Herbivore& to = herbivores.at_small_trans(
                        x, y, around[idx][0], around[idx][1]);
                    if (here.food < 10.) {
                        to.food = here.food;
                        here.food = 0.;
                    } else {
                        here.food /= 5.;
                        to.food = here.food;
                        here.moved = true;
                    }
                    to.moved = true;
                }
            }
        }
    }
}

void World::simulate()
{
    Grid<float> next(get_width(), get_height());
    disperse(temperature, 0.15, next);
    disperse(plants, 0.02, next);
    disperse(water, 0.02, next);
    disperse(clouds, 0.1, next);
    breed_plants(plants, temperature, water, clouds);
    vaporize(temperature, plants, water, clouds);
    precipitate(clouds, water);
    cool_down(temperature);
    produce_body_heat(plants, temperature);
    eat_plants(herbivores, plants);
    remove_move_markers(herbivores);
    move_herbivores(herbivores, plants);
}

static unsigned char amount2color(float amount)
{
    amount *= 10.;
    return amount > 255. ? 255 : amount;
}

void World::draw(SDL_Renderer* renderer)
{
    SDL_Rect viewport;
    SDL_RenderGetViewport(renderer, &viewport);
    int tile_width = viewport.w / get_width();
    int tile_height = viewport.h / get_height();
    for (size_t x = 0; x < get_width(); ++x) {
        for (size_t y = 0; y < get_height(); ++y) {
            SDL_Rect tile;
            tile.x = (int)x * tile_width;
            tile.y = (int)y * tile_height;
            tile.w = tile_width;
            tile.h = tile_height;
            SDL_SetRenderDrawColor(renderer,
                amount2color(temperature.at(x, y) * 2.),
                amount2color(plants.at(x, y) * 3.),
                amount2color(water.at(x, y) * 2.), 255);
            if (herbivores.at(x, y).food > 0.) {
                tile.x += 1;
                tile.y += 1;
                tile.w -= 2;
                tile.h -= 2;
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &tile);
            }
            SDL_RenderFillRect(renderer, &tile);
        }
    }
}
