#include "surface.h"

Surface::Surface()
{
    ambient_color = Color(0.0f, 0.0f, 0.0f);
    diffuse_color = Color(0.0f, 0.0f, 0.0f);
    specular_color = Color(0.0f, 0.0f, 0.0f);
    phong_exponent = 1;
    textured = false;
}

Surface::Surface(Vec3 p, Color c, int phong, bool t) :
    position(p),
    ambient_color(c),
    diffuse_color(c),
    phong_exponent(phong),
    textured(t)
{
    specular_color = Color(0.5f, 0.5f, 0.5f);
}

Surface::~Surface() {}

Color Surface::get_ambient_color(Vec3 point)
{
    if (textured) {
        return checkerboard_texture(point);
    }
    return ambient_color;
}

Color Surface::get_diffuse_color(Vec3 point)
{
    if (textured) {
        return checkerboard_texture(point);
    }
    return diffuse_color;
}

Color Surface::get_specular_color(Vec3 point)
{
    return specular_color;
}

int Surface::get_phong_exponent()
{
    return phong_exponent;
}

Color Surface::checkerboard_texture(Vec3 point)
{
    const float scale = 2;
    float x = point[0] / scale;
    float y = point[2] / scale;
    if (((x - std::floor(x)) < 0.5) ^ ((y - std::floor(y)) < 0.5)) {
        return Color(0.0f, 0.0f, 0.0f);
    }
    return Color(1.0f, 1.0f, 1.0f);
}
