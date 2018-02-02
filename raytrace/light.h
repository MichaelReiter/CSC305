#ifndef LIGHT_H
#define LIGHT_H

#include "OpenGP/Image/Image.h"

using namespace OpenGP;

class Light {
public:
    Light(const Vec3& p, double i);

    ~Light();

    Vec3 get_position() const;
    float get_intensity() const;

private:
    Vec3 position;
    double intensity;
};

#endif // LIGHT_H
