#include "surface.h"
#include "colors.h"

Surface::Surface() :
    m_ambient_color(Colors::black),
    m_diffuse_color(Colors::black),
    m_specular_color(Colors::black),
    m_phong_exponent(1),
    m_textured(false)
{}

Surface::Surface(const Vec3& p, const Color& c, int phong, bool t) :
    m_position(p),
    m_ambient_color(c),
    m_diffuse_color(c),
    m_specular_color(Colors::grey),
    m_phong_exponent(phong),
    m_textured(t)
{}

Surface::~Surface() {}

Color Surface::get_ambient_color(const Vec3& point) const
{
    if (m_textured) {
        return checkerboard_texture(point);
    }
    return m_ambient_color;
}

Color Surface::get_diffuse_color(const Vec3& point) const
{
    if (m_textured) {
        return checkerboard_texture(point);
    }
    return m_diffuse_color;
}

Color Surface::get_specular_color(const Vec3& point) const
{
    return m_specular_color;
}

int Surface::get_phong_exponent() const
{
    return m_phong_exponent;
}

Color Surface::checkerboard_texture(const Vec3& point) const
{
    const float scale = 2;
    float x = point[0] / scale;
    float y = point[2] / scale;
    if (((x - std::floor(x)) < 0.5) ^ ((y - std::floor(y)) < 0.5)) {
        return Colors::black;
    }
    return Colors::white;
}
