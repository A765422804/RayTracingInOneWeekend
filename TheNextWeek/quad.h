#pragma once

#include "hittable.h"

class quad : public hittable
{
public:
    quad(const point3 &Q, const vec3 &u, const vec3 &v, shared_ptr<material> mat)
        : Q(Q), u(u), v(v), mat(mat)
    {
        auto n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n, n);

        set_bounding_box();
    }

    virtual void set_bounding_box()
    {
        auto bbox1 = aabb(Q, Q + u + v);
        auto bbox2 = aabb(Q + u, Q + v);
        bbox = aabb(bbox1, bbox2);
    }

    aabb bounding_box() const override { return bbox; }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        auto demon = dot(normal, r.direction());

        if (fabs(demon) < 1e-8)
            return false;

        auto t = (D - dot(normal, r.origin())) / demon;

        if (!ray_t.contains(t))
            return false;

        auto intersection = r.at(t);
        vec3 p = intersection - Q;
        auto alpha = dot(w, cross(p, v));
        auto beta = dot(w, cross(u, p));

        if (!is_interior(alpha, beta, rec))
            return false;

        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

    virtual bool is_interior(double alpha, double beta, hit_record &rec) const
    {
        interval unit_interval(0, 1);

        if (!unit_interval.contains(alpha) || !unit_interval.contains(beta))
            return false;

        rec.u = alpha;
        rec.v = beta;
        
        return true;
    }

private:
    point3 Q;
    vec3 u, v;
    shared_ptr<material> mat;
    aabb bbox;
    vec3 normal;
    double D;
    vec3 w;
};