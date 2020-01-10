#include "Config.hpp"
#include <ctype.h>
#include <fstream>
#include <stddef.h>
#include <string>

int Config::parse(const char* path)
{
    // XXX: This parser is bad!
    std::string line;
    std::ifstream file;
    file.open(path, std::ifstream::in);
    std::getline(file, line);
    while (file.good()) {
        if (!line.empty() && line[0] != '#') {
            size_t key_len;
            for (key_len = 0; key_len < line.length(); ++key_len) {
                if (isspace(line[key_len])) {
                    break;
                }
            }
            std::string key = line.substr(0, key_len);
            float val = atof(line.c_str() + key_len + 1);
            if (key == "clouds_dispersal") {
                clouds_dispersal = val;
            } else if (key == "clouds_humidity_decrement") {
                clouds_humidity_decrement = val;
            } else if (key == "clouds_initial_max") {
                clouds_initial_max = val;
            } else if (key == "clouds_max_humidity") {
                clouds_max_humidity = val;
            } else if (key == "herbivores_initial_chance") {
                herbivores_initial_chance = val;
            } else if (key == "herbivores_initial_food") {
                herbivores_initial_food = val;
            } else if (key == "herbivores_plant_food_mul") {
                herbivores_plant_food_mul = val;
            } else if (key == "herbivores_food_decrement") {
                herbivores_food_decrement = val;
            } else if (key == "herbivores_baby_threshold") {
                herbivores_baby_threshold = val;
            } else if (key == "herbivores_birth_food_mul") {
                herbivores_birth_food_mul = val;
            } else if (key == "plants_body_heat_mul") {
                plants_body_heat_mul = val;
            } else if (key == "plants_dispersal") {
                plants_dispersal = val;
            } else if (key == "plants_initial_max") {
                plants_initial_max = val;
            } else if (key == "plants_overpopulation_mul") {
                plants_overpopulation_mul = val;
            } else if (key == "plants_overcast_mul") {
                plants_overcast_mul = val;
            } else if (key == "plants_temperature_excess_mul") {
                plants_temperature_excess_mul = val;
            } else if (key == "plants_temperature_lack_mul") {
                plants_temperature_lack_mul = val;
            } else if (key == "plants_water_lack_mul") {
                plants_water_lack_mul = val;
            } else if (key == "temperature_dispersal") {
                temperature_dispersal = val;
            } else if (key == "temperature_initial_max") {
                temperature_initial_max = val;
            } else if (key == "temperature_loss_mul") {
                temperature_loss_mul = val;
            } else if (key == "water_dispersal") {
                water_dispersal = val;
            } else if (key == "water_initial_max") {
                water_initial_max = val;
            } else if (key == "water_evaporation_temperature_mul") {
                water_evaporation_temperature_mul = val;
            } else if (key == "water_evaporation_plants_mul") {
                water_evaporation_plants_mul = val;
            } else {
                // Ignore for now.
            }
        }
        std::getline(file, line);
    }
    return 0;
}
