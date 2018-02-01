#ifndef MATERIAL_H
#define MATERIAL_H

#include "OpenGP/Image/Image.h"

using namespace OpenGP;
using Colour = Vec3;

class Material {
public:
    Material();
    Material(Color a, Color d, Color s, int p);

    ~Material();

    Color getAmbientColor();
    Color getDiffuseColor();
    Color getSpecularColor();
    int getPhongExponent();

private:
    Color ambientColor;
    Color diffuseColor;
    Color specularColor;
    int phongExponent;
};

#endif // MATERIAL_H
