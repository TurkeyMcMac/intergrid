#include "Config.hpp"
#include <ctype.h>
#include <fstream>
#include <stddef.h>
#include <stdlib.h>
#include <string>

static size_t skip_space(std::string& line, size_t i)
{
    while (i < line.length() && isspace(line[i])) {
        ++i;
    }
    if (i < line.length() && line[i] == '#') {
        return line.length();
    } else {
        return i;
    }
}

static bool is_key_char(int ch) { return isalnum(ch) || ch == '_'; }

static bool set_option(Config& conf, std::string& key, float val)
{
    if (key == "clouds_dispersal") {
        conf.clouds_dispersal = val;
    } else if (key == "clouds_humidity_decrement") {
        conf.clouds_humidity_decrement = val;
    } else if (key == "clouds_initial_max") {
        conf.clouds_initial_max = val;
    } else if (key == "clouds_max_humidity") {
        conf.clouds_max_humidity = val;
    } else if (key == "herbivores_initial_chance") {
        conf.herbivores_initial_chance = val;
    } else if (key == "herbivores_initial_food") {
        conf.herbivores_initial_food = val;
    } else if (key == "herbivores_plant_food_mul") {
        conf.herbivores_plant_food_mul = val;
    } else if (key == "herbivores_food_decrement") {
        conf.herbivores_food_decrement = val;
    } else if (key == "herbivores_baby_threshold") {
        conf.herbivores_baby_threshold = val;
    } else if (key == "herbivores_birth_food_mul") {
        conf.herbivores_birth_food_mul = val;
    } else if (key == "plants_body_heat_mul") {
        conf.plants_body_heat_mul = val;
    } else if (key == "plants_dispersal") {
        conf.plants_dispersal = val;
    } else if (key == "plants_initial_max") {
        conf.plants_initial_max = val;
    } else if (key == "plants_overpopulation_mul") {
        conf.plants_overpopulation_mul = val;
    } else if (key == "plants_overcast_mul") {
        conf.plants_overcast_mul = val;
    } else if (key == "plants_temperature_excess_mul") {
        conf.plants_temperature_excess_mul = val;
    } else if (key == "plants_temperature_lack_mul") {
        conf.plants_temperature_lack_mul = val;
    } else if (key == "plants_water_lack_mul") {
        conf.plants_water_lack_mul = val;
    } else if (key == "temperature_dispersal") {
        conf.temperature_dispersal = val;
    } else if (key == "temperature_initial_max") {
        conf.temperature_initial_max = val;
    } else if (key == "temperature_loss_mul") {
        conf.temperature_loss_mul = val;
    } else if (key == "water_dispersal") {
        conf.water_dispersal = val;
    } else if (key == "water_initial_max") {
        conf.water_initial_max = val;
    } else if (key == "water_evaporation_temperature_mul") {
        conf.water_evaporation_temperature_mul = val;
    } else if (key == "water_evaporation_plants_mul") {
        conf.water_evaporation_plants_mul = val;
    } else {
        return false;
    }
    return true;
}

int Config::parse(const char* path)
{
    // XXX: This parser is bad!
    std::string line;
    std::ifstream file;
    file.open(path, std::ifstream::in);
    for (std::getline(file, line); file.good(); std::getline(file, line)) {
        size_t i = skip_space(line, 0);
        if (i >= line.length()) {
            continue;
        }
        size_t key_start = i;
        size_t key_end;
        for (key_end = i; key_end < line.length() && is_key_char(line[key_end]);
             ++key_end) {
        }
        i = skip_space(line, key_end);
        if (i >= line.length() || line[i] != '=') {
            fprintf(
                stderr, "No configuration value on line: %s\n", line.c_str());
            return -1;
        }
        i = skip_space(line, i + 1);
        char* val_end;
        const char* val_start = line.c_str() + i;
        float val = strtod(val_start, &val_end);
        if (val_start == val_end) {
            fprintf(
                stderr, "No configuration value on line: %s\n", line.c_str());
            return -1;
        }
        if (*val_end != '\0' && *val_end != '#' && !isspace(*val_end)) {
            fprintf(stderr, "Invalid configuration value: %s\n", val_start);
            return -1;
        }
        std::string key = line.substr(key_start, key_end - key_start);
        if (!set_option(*this, key, val)) {
            fprintf(stderr, "Unknown configuration option: %s\n", key.c_str());
            return -1;
        }
    }
    return 0;
}
