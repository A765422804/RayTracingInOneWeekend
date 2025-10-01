#pragma once

#include <cmath>
#include <limits>
#include <memory>
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

// common headers

#include "vec3.h"
#include "ray.h"
#include "color.h"