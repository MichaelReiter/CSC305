#ifndef LIGHT_H
#define LIGHT_H

#include "OpenGP/Image/Image.h"

using namespace OpenGP;

class Light {
public:
    Light(Vec3 p, double i);

    ~Light();

    Vec3 getPosition();
    float getIntensity();

private:
    Vec3 position;
    double intensity;
};

#endif // LIGHT_H
