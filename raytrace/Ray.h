#ifndef RAY_H
#define RAY_H

#include "OpenGP/Image/Image.h"

using namespace OpenGP;

class Ray {
public:
    Vec3 origin;
    Vec3 direction;
    Ray(Vec3 o, Vec3 d);
    Vec3 position(double t);
};

#endif // RAY_H
