#include "Material.h"

using namespace OpenGP;

Material::Material() {
    ambientColor = Color(0.0f, 0.0f, 0.0f);
    diffuseColor = Color(0.0f, 0.0f, 0.0f);
    specularColor = Color(0.0f, 0.0f, 0.0f);
    phongExponent = 1;
};

Material::Material(Color a, Color d, Color s, int p) {
    ambientColor = a;
    diffuseColor = d;
    specularColor = s;
    phongExponent = p;
};

Material::Material(const Material &material) {
    this->ambientColor = material.ambientColor;
    this->diffuseColor = material.diffuseColor;
    this->specularColor = material.specularColor;
    this->phongExponent = material.phongExponent;
}
