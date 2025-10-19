#pragma once

#include "hittable_list.h"
#include "material.h"

class camera
{
public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;

    double fov = 90;
    point3 lookfrom = point3(0, 0, 0);
    point3 lookat = point3(0, 0, -1);
    vec3 vup = vec3(0, 1, 0);

    double defocus_angle = 0.0;
    double focus_dist = 10;

    color background;

    void render(const hittable &world)
    {
        initialize();

        std::cout << "P3\n"
                  << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; ++j)
        {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i)
            {
                color pixel_color = color(0, 0, 0);
                for (int s_j = 0; s_j < sqrt_spp; ++s_j)
                {
                    for (int s_i = 0; s_i < sqrt_spp; ++s_i)
                    {
                        ray r = get_ray(i, j, s_i, s_j);
                        pixel_color += ray_color(r, max_depth, world);
                    }
                }
                write_color(std::cout, pixel_color * pixel_samples_scale);
            }
        }

        std::clog << "\rDone.                 \n";
    }

private:
    int image_height;
    point3 center;
    point3 pixle00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    double pixel_samples_scale;
    vec3 u, v, w;
    vec3 defocus_disk_u, defocus_disk_v;

    int sqrt_spp;          // sqrt(sample per pixel)
    double recip_sqrt_spp; // 1 / sqrt(sample per pixel)

    void initialize()
    {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        sqrt_spp = int(std::sqrt(samples_per_pixel));
        pixel_samples_scale = 1.0 / (sqrt_spp * sqrt_spp);
        recip_sqrt_spp = 1.0 / sqrt_spp;

        center = lookfrom;

        auto theta = degrees_to_radians(fov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * -v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left = center - focus_dist * w - viewport_u / 2 - viewport_v / 2;
        pixle00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = defocus_radius * u;
        defocus_disk_v = defocus_radius * v;
    }

    color ray_color(const ray &r, int depth, const hittable &world)
    {
        if (depth <= 0)
            return color(0, 0, 0);

        hit_record rec;

        if (!world.hit(r, interval(0.001, infinity), rec))
            return background;

        ray scattered;
        color attenuation;
        double pdf_value;

        // 打到自发光材质
        color color_from_emission = rec.mat->emitted(r, rec, rec.u, rec.v, rec.p);

        if (!rec.mat->scatter(r, rec, attenuation, scattered, pdf_value))
            return color_from_emission;

        auto on_light = point3(random_double(213, 343), 554, random_double(227, 332));
        auto to_light = on_light - rec.p;
        auto distance_squared = to_light.length_squared();
        to_light = unit_vector(to_light);

        if (dot(to_light, rec.normal) < 0)
            return color_from_emission;
        
        double light_area = (343 - 213) * (332 - 227);
        auto light_cosine = std::fabs(to_light.y());
        if (light_cosine < 1e-6)
            return color_from_emission;

        pdf_value = distance_squared / (light_cosine * light_area);
        scattered = ray(rec.p, to_light, r.time());

        // 散射出去
        double scattering_pdf = rec.mat->scattering_pdf(r, rec, scattered);

        color color_from_scatter = attenuation * scattering_pdf * ray_color(scattered, depth - 1, world) / pdf_value;

        return color_from_emission + color_from_scatter;
    }

    ray get_ray(int i, int j, int s_i, int s_j) const
    {

        auto offset = sample_square_stratified(s_i, s_j);
        auto pixel_sample = pixle00_loc + ((i + offset.x()) * pixel_delta_u + (j + offset.y()) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;
        auto ray_time = random_double(); // 0-1之间的随机数

        return ray(ray_origin, ray_direction, ray_time);
    }

    vec3 sample_square_stratified(int s_i, int s_j) const
    {
        auto px = (s_i + random_double()) * recip_sqrt_spp - 0.5;
        auto py = (s_j + random_double()) * recip_sqrt_spp - 0.5;

        return vec3(px, py, 0);
    }

    vec3 sample_square() const
    {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    point3 defocus_disk_sample() const
    {
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }
};