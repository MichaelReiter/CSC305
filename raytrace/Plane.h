#ifndef PLANE_H
#define PLANE_H

#include "OpenGP/Image/Image.h"
#include "material.h"
#include "surface.h"

using namespace OpenGP;
using Ray = Eigen::ParametrizedLine<float, 3>;

class Plane : public Surface {
public:
    Plane(Vec3 p, Vec3 n, Material m);

    ~Plane();

    float get_ray_intersection_parameter(Ray ray);
    Vec3 get_normal(Vec3 point);

private:
    Vec3 normal;
};

#endif // PLANE_H
