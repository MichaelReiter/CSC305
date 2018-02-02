#include "light.h"

Light::Light(const Vec3& p, double i) :
    position(p),
    intensity(i)
{}

Light::~Light() {}

Vec3 Light::get_position() const
{
    return position;
}

float Light::get_intensity() const
{
    return intensity;
}
