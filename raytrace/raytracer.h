#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "surface.h"
#include "light.h"

class Raytracer {
public:
    Raytracer(int w, int h, int s);

    ~Raytracer();

    Color phong_lighting(
        const std::vector<Light>& lights,
        const Vec3& intersection_point,
        const Vec3& camera_position,
        const Surface& s,
        const std::vector<Surface*>& scene
    ) const;
    Image<Color> trace_rays() const;

private:
    int width_resolution;
    int height_resolution;
    int supersample_factor;

    Image<Color> apply_supersampling(Image<Color>& image) const;
};

#endif // RAYTRACER_H
