#ifndef SURFACE_H
#define SURFACE_H

#include "OpenGP/Image/Image.h"

using namespace OpenGP;
using Ray = Eigen::ParametrizedLine<float, 3>;
using Color = Vec3;

class Surface {
public:
    Surface();
    Surface(Vec3 p, Color c, int phong, bool t=false);

    ~Surface();

    virtual float get_ray_intersection_parameter(Ray ray) = 0;
    virtual Vec3 get_normal(Vec3 point) = 0;
    Color get_ambient_color(Vec3 point);
    Color get_diffuse_color(Vec3 point);
    Color get_specular_color(Vec3 point);
    int get_phong_exponent();

    Color checkerboard_texture(Vec3 point);

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
