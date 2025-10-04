#pragma once

#include "rtweekend.h"

class interval
{
public:
    double min, max;

    interval() : min(+infinity), max(-infinity) {}

    interval(double min, double max) : min(min), max(max) {}

    interval(const interval& a, const interval& b)
    {
        //创建一个 interval，其范围是 a 和 b 的最小和最大值
        min = a.min < b.min ? a.min : b.min;
        max = a.max > b.max ? a.max : b.max;
    }

    double size() const { return max - min; }

    bool contains(double x) const { return x >= min && x <= max; }

    bool surrounds(double x) const { return x > min && x < max; }

    double clamp(double x) const { return x < min ? min : (x > max ? max : x); }

    interval expand(double delta) const
    {
        auto padding = delta / 2;
        return interval(min - padding, max + padding);
    }

    static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);