//
// Created by Michael Reiter on 2018-01-18.
//

#include "OpenGP/Image/Image.h"

#ifndef ICG_SPHERE_H
#define ICG_SPHERE_H

using namespace OpenGP;

class Sphere {
    Vec3 center;
    double radius;
public:
    Sphere(Vec3 c, double r);
};


#endif //ICG_SPHERE_H
