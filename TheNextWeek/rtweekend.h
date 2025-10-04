#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
#include <iostream>

// c++ std using
using std::shared_ptr;
using std::make_shared;

// constant

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.14159265358979323846;

// utility functions

inline double degrees_to_radians(double degrees) 
{
    return degrees * pi / 180.0;
}

inline double random_double()
{
    // [0, 1) range
    return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
    // [min, max) range
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max)
{
    return int(random_double(min, max + 1.0));
}

// common headers

#include "vec3.h"
#include "ray.h"
#include "color.h"