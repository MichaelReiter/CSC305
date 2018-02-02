#include "material.h"

Material::Material()
{
    ambient_color = Color(0.0f, 0.0f, 0.0f);
    diffuse_color = Color(0.0f, 0.0f, 0.0f);
    specular_color = Color(0.0f, 0.0f, 0.0f);
    phong_exponent = 1;
    textured = false;
};

Material::Material(Color c, int p, bool t) :
    ambient_color(c),
    diffuse_color(c),
    phong_exponent(p),
    textured(t)
{
    specular_color = Color(0.5f, 0.5f, 0.5f);
}

Material::~Material() {}

Color Material::get_ambient_color(Vec3 point)
{
    if (textured) {
        return checkerboard_texture(point);
    }
    return ambient_color;
}

Color Material::get_diffuse_color(Vec3 point)
{
    if (textured) {
        return checkerboard_texture(point);
    }
    return diffuse_color;
}

Color Material::get_specular_color(Vec3 point)
{
    return specular_color;
}

int Material::get_phong_exponent()
{
    return phong_exponent;
}

float modulo(float x) 
{ 
    return x - std::floor(x); 
}

Color Material::checkerboard_texture(Vec3 point)
{
    float scale = 2;
    if ((modulo(point[0] / scale) < 0.5) ^ (modulo(point[2] / scale) < 0.5)) {
        return Color(0.0f, 0.0f, 0.0f);
    }
    return Color(1.0f, 1.0f, 1.0f);
}
