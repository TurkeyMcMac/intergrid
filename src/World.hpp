#ifndef WORLD_H_
#define WORLD_H_

#include "Config.hpp"
#include "Grid.hpp"
#include <SDL2/SDL.h>

namespace intergrid {

struct Herbivore {
    float food;
    bool moved;
};

class World {
public:
    World(size_t width, size_t height);

    size_t get_width();

    size_t get_height();

    void randomize(Config& conf);

    void simulate(Config& conf);

    void draw(SDL_Renderer* renderer);

    double count_total_temperature();

    double count_total_plants();

    double count_total_water();

    double count_total_clouds();

    long count_total_herbivores();

    double count_total_herbivore_food();

private:
    Grid<float> temperature;
    Grid<float> plants;
    Grid<float> water;
    Grid<float> clouds;
    Grid<Herbivore> herbivores;
};

} /* namespace intergrid */

#endif /* WORLD_H_ */
