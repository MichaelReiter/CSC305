#include "Material.h"

Material::Material() {
    ambientColor = Color(0.0f, 0.0f, 0.0f);
    diffuseColor = Color(0.0f, 0.0f, 0.0f);
    specularColor = Color(0.0f, 0.0f, 0.0f);
    phongExponent = 1;
};

Material::Material(Color a, Color d, Color s, int p) :
    ambientColor(a),
    diffuseColor(d),
    specularColor(s),
    phongExponent(p)
{}

Material::~Material() {}

Color Material::getAmbientColor() {
    return ambientColor;
}

Color Material::getDiffuseColor() {
    return diffuseColor;
}

Color Material::getSpecularColor() {
    return specularColor;
}

int Material::getPhongExponent() {
    return phongExponent;
}
