#ifndef SPHERE_H
#define SPHERE_H

#include "OpenGP/Image/Image.h"
#include "surface.h"

using namespace OpenGP;
using Ray = Eigen::ParametrizedLine<float, 3>;

class Sphere : public Surface {
public:
    Sphere(Vec3 p, float r, Color c, int phong, bool t=false);

    ~Sphere();

    float get_ray_intersection_parameter(Ray ray);
    Vec3 get_normal(Vec3 point);

private:
    float radius;
};

#endif // SPHERE_H
