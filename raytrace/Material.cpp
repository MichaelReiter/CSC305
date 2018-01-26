#include "Material.h"

using namespace OpenGP;

Material::Material() {
    diffuseColor = Color(0.0f, 0.0f, 0.0f);
    specularColor = Color(1.0f, 1.0f, 1.0f);
    phongExponent = 1;
};

Material::Material(Color d, Color s, int p) {
    diffuseColor = d;
    specularColor = s;
    phongExponent = p;
};

Material::Material(const Material &material) {
    this->diffuseColor = material.diffuseColor;
    this->specularColor = material.specularColor;
    this->phongExponent = material.phongExponent;
}
