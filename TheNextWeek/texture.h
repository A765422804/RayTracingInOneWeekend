#pragma once

#include "color.h"

class texture
{
public:
    virtual ~texture() {}

    virtual color value(double u, double v, const vec3 &p) const = 0;
};

class solid_color : public texture
{
public:
    solid_color(const color &c) : albedo(c) {}

    solid_color(double red, double green, double blue) : albedo(red, green, blue) {}

    color value(double u, double v, const vec3 &p) const { return albedo; }

private:
    color albedo;
};

class checker_texture : public texture
{
public:
    checker_texture(double scale, shared_ptr<texture> odd, shared_ptr<texture> even)
        : inv_scale(1 / scale), odd(odd), even(even) {}
    
    checker_texture(double scale, const color &odd, const color &even): checker_texture(scale, make_shared<solid_color>(odd), make_shared<solid_color>(even)) {}

    color value(double u, double v, const vec3 &p) const
    {
        auto xInteger = int(std::floor(p.x() * inv_scale));
        auto yInteger = int(std::floor(p.y() * inv_scale));
        auto zInteger = int(std::floor(p.z() * inv_scale));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }
private:
    double inv_scale;
    shared_ptr<texture> odd;
    shared_ptr<texture> even;
};