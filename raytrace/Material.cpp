#include "material.h"

Material::Material()
{
    ambient_color = Color(0.0f, 0.0f, 0.0f);
    diffuse_color = Color(0.0f, 0.0f, 0.0f);
    specular_color = Color(0.0f, 0.0f, 0.0f);
    phong_exponent = 1;
};

Material::Material(Color a, Color d, Color s, int p) :
    ambient_color(a),
    diffuse_color(d),
    specular_color(s),
    phong_exponent(p)
{}

Material::~Material() {}

Color Material::get_ambient_color()
{
    return ambient_color;
}

Color Material::get_diffuse_color()
{
    return diffuse_color;
}

Color Material::get_specular_color()
{
    return specular_color;
}

int Material::get_phong_exponent()
{
    return phong_exponent;
}
