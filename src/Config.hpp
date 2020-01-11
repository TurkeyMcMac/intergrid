#ifndef CONFIG_H_
#define CONFIG_H_

namespace intergrid {

struct Config {
public:
    // Initializes the default values. See the file "conf" for said defaults.
    Config();

    int parse(const char* path);

    float clouds_dispersal;
    float clouds_humidity_decrement;
    float clouds_initial_max;
    float clouds_max_humidity;
    float herbivores_initial_chance;
    float herbivores_initial_food;
    float herbivores_plant_food_mul;
    float herbivores_food_decrement;
    float herbivores_baby_threshold;
    float herbivores_birth_food_mul;
    float plants_body_heat_mul;
    float plants_dispersal;
    float plants_initial_max;
    float plants_overpopulation_mul;
    float plants_overcast_mul;
    float plants_temperature_excess_mul;
    float plants_temperature_lack_mul;
    float plants_water_lack_mul;
    float temperature_dispersal;
    float temperature_initial_max;
    float temperature_loss_mul;
    float water_dispersal;
    float water_initial_max;
    float water_evaporation_temperature_mul;
    float water_evaporation_plants_mul;
    float world_height;
    float world_width;
};

} /* namespace intergrid */

#endif /* CONFIG_H_ */
