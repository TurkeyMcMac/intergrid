#include "Options.hpp"
#include <limits.h>
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

static int get_pos_num_arg(char* argv[], int& i)
{
    char* arg_string = get_arg(argv, i);
    char* end;
    long arg_long = strtol(arg_string, &end, 10);
    if (*end != '\0') {
        fprintf(stderr, "%s: Invalid numeric argument to option %s: %s\n",
            argv[0], argv[i - 1], arg_string);
        exit(EXIT_FAILURE);
    }
    if (arg_long <= 0 || arg_long > INT_MAX) {
        fprintf(stderr, "%s: Numeric argument to option %s out of range: %s\n",
            argv[0], argv[i - 1], arg_string);
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
    printf("%s help placeholder\n", progname);
}

static void print_version(char* progname)
{
    printf("%s version placeholder\n", progname);
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
            frame_delay = (unsigned)get_pos_num_arg(argv, i);
        } else if (!strcmp(opt, "-screen-width")) {
            screen_width = get_pos_num_arg(argv, i);
        } else if (!strcmp(opt, "-screen-height")) {
            screen_height = get_pos_num_arg(argv, i);
        } else if (!strcmp(opt, "-pixel-size")) {
			int size = get_pos_num_arg(argv, i);
            pixel_width = size;
            pixel_height = size;
        } else if (!strcmp(opt, "-pixel-width")) {
            pixel_width = get_pos_num_arg(argv, i);
        } else if (!strcmp(opt, "-pixel-height")) {
            pixel_height = get_pos_num_arg(argv, i);
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
        } else if (!strcmp(argv[i] /* No leading '-' removed */, "--")) {
            // "--" ends option parsing.
            break;
        } else {
            fprintf(stderr, "%s: Invalid option: %s\n", progname, argv[i]);
            print_usage(progname, stderr);
            fprintf(stderr, "Type %s -help for more information\n", progname);
            exit(EXIT_FAILURE);
        }
    }
}
