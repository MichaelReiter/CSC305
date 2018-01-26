#ifndef MATERIAL_H
#define MATERIAL_H

#include "OpenGP/Image/Image.h"

using namespace OpenGP;
using Colour = Vec3;

class Material {
public:
    Color diffuseColor;
    Color specularColor;
    int phongExponent;
    Material();
    Material(Color d, Color s, int p);
    Material(const Material& material);
};

#endif
