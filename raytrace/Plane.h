#ifndef PLANE_H
#define PLANE_H

#include "OpenGP/Image/Image.h"
#include "Material.h"
#include "Ray.h"
#include "Surface.h"

using namespace OpenGP;

class Plane : public Surface {
public:
    Vec3 normal;
    Plane(Vec3 p, Vec3 n, Material m);
    float hit(Ray ray);
};

#endif
