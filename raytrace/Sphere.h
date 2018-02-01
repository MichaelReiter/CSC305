#ifndef SPHERE_H
#define SPHERE_H

#include "OpenGP/Image/Image.h"
#include "Material.h"
#include "Surface.h"

using namespace OpenGP;
using Ray = Eigen::ParametrizedLine<float, 3>;

class Sphere : public Surface {
public:
    float radius;

    Sphere(Vec3 p, float r, Material m) :
        Surface(p, m),
        radius(r)
    {}

    ~Sphere();

    float hit(Ray ray);
    Vec3 getNormalAtPoint(Vec3 point);
};

#endif // SPHERE_H
