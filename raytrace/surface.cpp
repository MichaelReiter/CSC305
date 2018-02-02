#include "surface.h"
#include "colors.h"

Surface::Surface()
{
    ambient_color = Colors::black;
    diffuse_color = Colors::black;
    specular_color = Colors::black;
    phong_exponent = 1;
    textured = false;
}

Surface::Surface(const Vec3& p, const Color& c, int phong, bool t) :
    position(p),
    ambient_color(c),
    diffuse_color(c),
    phong_exponent(phong),
    textured(t)
{
    specular_color = Colors::grey;
}

Surface::~Surface() {}

Color Surface::get_ambient_color(const Vec3& point) const
{
    if (textured) {
        return checkerboard_texture(point);
    }
    return ambient_color;
}

Color Surface::get_diffuse_color(const Vec3& point) const
{
    if (textured) {
        return checkerboard_texture(point);
    }
    return diffuse_color;
}

Color Surface::get_specular_color(const Vec3& point) const
{
    return specular_color;
}

int Surface::get_phong_exponent() const
{
    return phong_exponent;
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
