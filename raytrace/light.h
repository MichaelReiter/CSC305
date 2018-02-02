#ifndef LIGHT_H
#define LIGHT_H

#include "OpenGP/Image/Image.h"

typedef OpenGP::Vec3 Vec3;

class Light {
public:
    Light(const Vec3& p, double i);

    ~Light();

    Vec3 get_position() const;
    float get_intensity() const;

private:
    Vec3 m_position;
    double m_intensity;
};

#endif // LIGHT_H
