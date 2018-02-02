#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "OpenGP/Image/Image.h"
#include "surface.h"
#include "light.h"

typedef OpenGP::Image<Color> Image;

class Raytracer {
public:
    Raytracer(int w, int h, int s);

    ~Raytracer();

    Image trace_rays() const;

private:
    int m_width_resolution;
    int m_height_resolution;
    int m_supersample_factor;

    Color phong_lighting(
        const std::vector<Light>& lights,
        const Vec3& intersection_point,
        const Vec3& camera_position,
        const Surface& s,
        const std::vector<Surface*>& scene
    ) const;
    Image apply_supersampling(Image& image) const;
};

#endif // RAYTRACER_H
