#ifndef SPHERE_H
#define SPHERE_H

#include "OpenGP/Image/Image.h"
#include "Material.h"
#include "Ray.h"
#include "Surface.h"

using namespace OpenGP;

class Sphere : public Surface {
public:
    float radius;
    Sphere(Vec3 p, float r, Material m);
    float hit(Ray ray);
    Vec3 getNormalAtPoint(Vec3 point);
};

#endif
