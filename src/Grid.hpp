#ifndef GRID_H_
#define GRID_H_

#include <stdlib.h>

namespace intergrid {

template <typename T> class Grid {
public:
    Grid(size_t width, size_t height)
        : width(width)
        , height(height)
        , tiles((T*)calloc(width * sizeof(T), height))
    {
    }

    Grid(size_t width, size_t height, T fill)
        : width(width)
        , height(height)
        , tiles((T*)malloc(width * height * sizeof(T)))
    {
        for (size_t i = 0; i < width * height; ++i) {
            tiles[i] = fill;
        }
    }

    ~Grid() { free(tiles); }

    T& at(size_t x, size_t y) { return tiles[y * width + x]; }

    void trans(size_t& x, size_t& y, int ox, int oy)
    {
        ox %= width;
        oy %= height;
        small_trans(x, y, ox, oy);
    }

    T& at_trans(size_t x, size_t y, int ox, int oy)
    {
        trans(x, y, ox, oy);
        return at(x, y);
    }

    void small_trans(size_t& x, size_t& y, int ox, int oy)
    {
        if (ox < 0) {
            size_t uox = -ox;
            if (x < uox) {
                x = width - (uox - x);
            } else {
                x -= uox;
            }
        } else {
            size_t uox = ox;
            x += uox;
            if (x >= width) {
                x -= width;
            }
        }
        if (oy < 0) {
            size_t uoy = -oy;
            if (y < uoy) {
                y = height - (uoy - y);
            } else {
                y -= uoy;
            }
        } else {
            size_t uoy = oy;
            y += uoy;
            if (y >= height) {
                y -= height;
            }
        }
    }

    T& at_small_trans(size_t x, size_t y, int ox, int oy)
    {
        small_trans(x, y, ox, oy);
        return at(x, y);
    }

    size_t get_width() { return width; }

    size_t get_height() { return height; }

private:
    size_t width;
    size_t height;
    T* tiles;
};

} /* namespace intergrid */

#endif /* GRID_H_ */
