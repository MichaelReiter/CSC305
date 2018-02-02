#ifndef SURFACE_H
#define SURFACE_H

#include "OpenGP/Image/Image.h"

typedef Eigen::ParametrizedLine<float, 3> Ray;
typedef OpenGP::Vec3 Vec3;
typedef Vec3 Color;

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
    Vec3 m_position;

private:
    Color m_ambient_color;
    Color m_diffuse_color;
    Color m_specular_color;
    int m_phong_exponent;
    bool m_textured;
};

#endif // SURFACE_H
