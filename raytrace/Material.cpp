#include "Material.h"

Material::Material() {
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

Color Material::GetAmbientColor() {
    return ambient_color;
}

Color Material::GetDiffuseColor() {
    return diffuse_color;
}

Color Material::GetSpecularColor() {
    return specular_color;
}

int Material::GetPhongExponent() {
    return phong_exponent;
}
