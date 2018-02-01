#include "light.h"

using namespace OpenGP;

Light::Light(Vec3 p, double i)
{
    position = p;
    intensity = i;
}

Light::~Light() {}

Vec3 Light::get_position()
{
    return position;
}

float Light::get_intensity()
{
    return intensity;
}
