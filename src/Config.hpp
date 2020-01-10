#ifndef CONFIG_H_
#define CONFIG_H_

struct Config {
public:
    Config()
        : clouds_dispersal(0.1)
        , clouds_humidity_decrement(0.1)
        , clouds_initial_max(10.)
        , clouds_max_humidity(6.)
        , herbivores_initial_chance(0.01)
        , herbivores_initial_food(10.)
        , herbivores_plant_food_mul(0.1)
        , herbivores_food_decrement(0.2)
        , herbivores_baby_threshold(10.)
        , herbivores_birth_food_mul(0.2)
        , plants_body_heat_mul(0.02)
        , plants_dispersal(0.02)
        , plants_initial_max(10.)
        , plants_overpopulation_mul(0.9)
        , plants_overcast_mul(0.001)
        , plants_temperature_excess_mul(1.06)
        , plants_temperature_lack_mul(0.99)
        , plants_water_lack_mul(0.93)
        , temperature_dispersal(0.15)
        , temperature_initial_max(10.)
        , temperature_loss_mul(0.99)
        , water_dispersal(0.02)
        , water_initial_max(10.)
        , water_evaporation_temperature_mul(0.01)
        , water_evaporation_plants_mul(0.01)
    {
    }

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
};

#endif /* CONFIG_H_ */
