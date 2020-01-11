# Intergrid

Intergrid is an ecological simulation screensaver. It simulates a bunch of
different quantities in different grids interacting. These quantities include
temperature, water, plants, and herbivores. I'm no biologist, but I figure
emergent phenomena arise out of many simple systems (the different grids)
interacting in convoluted ways. That is the philosophy behind this project.

## Installation

The dependencies of Intergrid are a C++03 compiler and standard library and the
SDL2 library. To compile, run this:

```
make
```

This will create an executable file `intergrid`. Run it like this:

```
./intergrid
```

You can get more detailed help by running this:

```
./intergrid -help
```

## Configuration

Intergrid has many different parameters to control the simulation. These are
documented in the file `conf`. To load a configuration file like `conf`, use the
`-conf` option for `./intergrid`.
