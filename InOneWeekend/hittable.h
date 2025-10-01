#pragma once

#include "interval.h"

class hit_record // 用于记录光线与物体相交信息的类
{
public:
    point3 p;    // hit position
    vec3 normal; // hit normal
    double t;    // hit time
    bool front_face; // 判断光线是从物体内部还是外部射入

    void set_face_normal(const ray &r, const vec3 &outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
    virtual ~hittable() {}

    virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;
};