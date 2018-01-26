#ifndef MATERIAL_H
#define MATERIAL_H

#include "OpenGP/Image/Image.h"

using namespace OpenGP;
using Colour = Vec3;

class Material {
public:
    Color ambientColor;
    Color diffuseColor;
    Color specularColor;
    int phongExponent;
    Material();
    Material(Color a, Color d, Color s, int p);
    Material(const Material& material);
};

#endif
