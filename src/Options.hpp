#ifndef OPTIONS_H_
#define OPTIONS_H_

#include "Config.hpp"

namespace intergrid {

struct Options {
public:
    Options(int argc, char* argv[]);

    Config conf;
    bool draw;
    bool print_stats;
    unsigned frame_delay;
    int screen_width;
    int screen_height;
    int pixel_width;
    int pixel_height;
    bool fullscreen;
};

} /* namespace intergrid */

#endif /* OPTIONS_H_ */
