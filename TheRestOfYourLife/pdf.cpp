#pragma once

#include "vec3.h"
#include "rtweekend.h"
#include "onb.h"

class pdf
{
public:
    virtual ~pdf() {}

    // 返回给定方向上的概率密度的值
    virtual double value(const vec3 &direction) const = 0;

    // 从 pdf 中采样一个方向
    virtual vec3 generate() const = 0;
};

class sphere_pdf : public pdf
{
public:
    sphere_pdf() {}

    double value(const vec3 &direction) const override
    {
        return 1.0 / (4.0 * pi);
    }

    vec3 generate() const override
    {
        return random_unit_vector();
    }
};

class cosine_pdf : public pdf
{
public:
    cosine_pdf(const vec3& w) : uvw(w) {}

    double value(const vec3 &direction) const override
    {
        double cos_theta = dot(direction, uvw.w());
        return std::fmax(0.0, cos_theta / pi);
    }

    vec3 generate() const override
    {
        return uvw.transform(random_cosine_direction());
    }
private:
    onb uvw;
};