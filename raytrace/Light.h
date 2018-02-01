#ifndef LIGHT_H
#define LIGHT_H

#include "OpenGP/Image/Image.h"

using namespace OpenGP;

class Light {
public:
    Vec3 position;
    double intensity;
    Light(Vec3 p, double i);
};

#endif // LIGHT_H
