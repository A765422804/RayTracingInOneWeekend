#pragma once

#include "color.h"
#include "rtw_stb_image.h"

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

    checker_texture(double scale, const color &odd, const color &even) : checker_texture(scale, make_shared<solid_color>(odd), make_shared<solid_color>(even)) {}

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

class image_texture : public texture
{
public:
    image_texture(const char *filename) : image(filename) {}

    color value(double u, double v, const vec3 &p) const
    {
        if (image.height() <= 0)
            return color(0, 1, 1);

        u = interval(0, 1).clamp(u);
        v = 1 - interval(0, 1).clamp(v); // Flip V to image coordinates

        auto i = int(u * image.width());
        auto j = int(v * image.height());
        auto pixel = image.pixel_data(i, j);

        auto color_scale = 1.0 / 255.0;
        return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

private:
    rtw_image image;
};