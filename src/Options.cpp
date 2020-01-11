#include "Options.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace intergrid;

static char* get_arg(char* argv[], int& i)
{
    ++i;
    if (!argv[i]) {
        fprintf(stderr, "%s: Missing argument to option %s\n", argv[0],
            argv[i - 1]);
        exit(EXIT_FAILURE);
    }
    return argv[i];
}

static int get_num_arg(char* argv[], int& i, int lower, int upper)
{
    char* arg_string = get_arg(argv, i);
    char* end;
    long arg_long = strtol(arg_string, &end, 10);
    if (*end != '\0') {
        fprintf(stderr, "%s: Invalid numeric argument %s to option %s\n",
            argv[0], arg_string, argv[i - 1]);
        exit(EXIT_FAILURE);
    }
    if (arg_long < (long)lower || arg_long > (long)upper) {
        fprintf(stderr,
            "%s: Numeric argument %s to option %s out of range %d to %d\n",
            argv[0], arg_string, argv[i - 1], lower, upper);
        exit(EXIT_FAILURE);
    }
    return (int)arg_long;
}

static void print_usage(char* progname, FILE* to)
{
    fprintf(to, "Usage: %s [options]\n", progname);
}

static void print_help(char* progname)
{
    print_usage(progname, stdout);
    puts("\
Options:\n\
 -conf <path>            Load the simulation configuration at <path>. The\n\
                         format is not described here.\n\
 -conf-str <conf>        Load configuration from the given string <conf>.\n\
 -draw                   Open a window and draw the world every frame.\n\
                         This is the default.\n\
 -no-draw                Do not open a window or draw the world.\n\
 -print-stats            Print statistics to stdout in the following order:\n\
                          1. Simulation tick.\n\
                          2. Sum of all tile temperatures.\n\
                          3. Sum of all tile plant levels.\n\
                          4. Sum of all tile water levels.\n\
                          5. Sum of all tile cloud levels.\n\
                          6. Number of herbivores alive.\n\
                          7. Sum of the food in all herbivore stomachs.\n\
                         All numbers are printed on one line.\n\
 -no-print-stats         Do not print statistics. This is the default.\n\
 -frame-delay <delay>    Delay for <delay> milliseconds per frame.\n\
 -screen-width <width>   Draw the screen <width> screen pixels wide.\n\
 -screen-height <height> Draw the screen <height> screen pixels tall.\n\
 -pixel-size <size>      Set the simulation pixel width and height. Pixel\n\
                         size is only effected if world_width/world_height is\n\
                         set to 0 in configuration, so that world_width/\n\
                         world_height is automatically decided.\n\
 -pixel-width <width>    Set the simulation pixel width.\n\
 -pixel-height <width>   Set the simulation pixel height.\n\
 -fullscreen             Draw the world over the entire screen.\n\
 -no-fullscreen          Draw the world on part of the screen.\n\
                         This is the default.\n\
 -help                   Print this help information.\n\
 -version                Print version information.");
}

static void print_version(char* progname)
{
    printf("%s version 0.1.1\n", progname);
}

Options::Options(int argc, char* argv[])
    : conf()
    , draw(true)
    , print_stats(false)
    , frame_delay(100)
    , screen_width(800)
    , screen_height(600)
    , pixel_width(10)
    , pixel_height(10)
    , fullscreen(false)
{
    char* progname = argv[0];
    for (int i = 1; i < argc; ++i) {
        char* opt = argv[i];
        if (opt[0] == '-' && opt[1] == '-') {
            ++opt;
        }
        if (!strcmp(opt, "-conf")) {
            char* file = get_arg(argv, i);
            if (conf.parse(file) < 0) {
                fprintf(stderr, "%s: Unable to parse configuration from '%s'\n",
                    progname, file);
                exit(EXIT_FAILURE);
            }
        } else if (!strcmp(opt, "-conf-str")) {
            char* str = get_arg(argv, i);
            if (conf.parse_cstr(str) < 0) {
                fprintf(stderr,
                    "%s: Unable to parse configuration string '%s'\n", progname,
                    str);
                exit(EXIT_FAILURE);
            }
        } else if (!strcmp(opt, "-draw")) {
            draw = true;
        } else if (!strcmp(opt, "-no-draw")) {
            draw = false;
        } else if (!strcmp(opt, "-print-stats")) {
            print_stats = true;
        } else if (!strcmp(opt, "-no-print-stats")) {
            print_stats = false;
        } else if (!strcmp(opt, "-frame-delay")) {
            frame_delay = (unsigned)get_num_arg(argv, i, 0, 2000000000);
        } else if (!strcmp(opt, "-screen-width")) {
            screen_width = get_num_arg(argv, i, 1, 1000000);
        } else if (!strcmp(opt, "-screen-height")) {
            screen_height = get_num_arg(argv, i, 1, 1000000);
        } else if (!strcmp(opt, "-pixel-size")) {
            int size = get_num_arg(argv, i, 1, 1000000);
            pixel_width = size;
            pixel_height = size;
        } else if (!strcmp(opt, "-pixel-width")) {
            pixel_width = get_num_arg(argv, i, 1, 1000000);
        } else if (!strcmp(opt, "-pixel-height")) {
            pixel_height = get_num_arg(argv, i, 1, 1000000);
        } else if (!strcmp(opt, "-fullscreen")) {
            fullscreen = true;
        } else if (!strcmp(opt, "-no-fullscreen")) {
            fullscreen = false;
        } else if (!strcmp(opt, "-help") || !strcmp(opt, "-h")) {
            print_help(progname);
            exit(EXIT_SUCCESS);
        } else if (!strcmp(opt, "-version") || !strcmp(opt, "-v")) {
            print_version(progname);
            exit(EXIT_SUCCESS);
        } else {
            fprintf(stderr, "%s: Invalid option: %s\n", progname, argv[i]);
            print_usage(progname, stderr);
            fprintf(stderr, "Type %s -help for more information\n", progname);
            exit(EXIT_FAILURE);
        }
    }
}
