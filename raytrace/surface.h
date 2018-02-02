#ifndef SURFACE_H
#define SURFACE_H

#include "OpenGP/Image/Image.h"

using namespace OpenGP;
using Ray = Eigen::ParametrizedLine<float, 3>;
using Color = Vec3;

class Surface {
public:
    Surface();
    Surface(const Vec3& p, const Color& c, int phong, bool t=false);

    ~Surface();

    virtual float get_ray_intersection_parameter(const Ray& ray) const = 0;
    virtual Vec3 get_normal(const Vec3& point) const = 0;
    Color get_ambient_color(const Vec3& point) const;
    Color get_diffuse_color(const Vec3& point) const;
    Color get_specular_color(const Vec3& point) const;
    int get_phong_exponent() const;

    Color checkerboard_texture(const Vec3& point) const;

protected:
    Vec3 position;

private:
    Color ambient_color;
    Color diffuse_color;
    Color specular_color;
    int phong_exponent;
    bool textured;
};

#endif // SURFACE_H
