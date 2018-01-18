//
// Created by Michael Reiter on 2018-01-18.
//

#include "OpenGP/Image/Image.h"

#ifndef ICG_RAY_H
#define ICG_RAY_H

using namespace OpenGP;

class Ray {
    Vec3 origin;
    Vec3 direction;
public:
    Ray(Vec3 o, Vec3 d);
    Vec3 position(double t);
};

#endif //ICG_RAY_H
