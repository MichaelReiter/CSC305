#ifndef SPHERE_H
#define SPHERE_H

#include "OpenGP/Image/Image.h"
#include "Material.h"
#include "Surface.h"

using namespace OpenGP;
using Ray = Eigen::ParametrizedLine<float, 3>;

class Sphere : public Surface {
public:
    Sphere(Vec3 p, float r, Material m);

    ~Sphere();

    float get_ray_intersection_parameter(Ray ray);
    Vec3 get_normal(Vec3 point);

private:
    float radius;
};

#endif // SPHERE_H
