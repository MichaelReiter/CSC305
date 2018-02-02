#include "light.h"

Light::Light(const Vec3& p, double i) :
    m_position(p),
    m_intensity(i)
{}

Light::~Light() {}

Vec3 Light::get_position() const
{
    return m_position;
}

float Light::get_intensity() const
{
    return m_intensity;
}
