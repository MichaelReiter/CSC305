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

    Color GetAmbientColor();
    Color GetDiffuseColor();
    Color GetSpecularColor();
    int GetPhongExponent();

private:
    Color ambient_color;
    Color diffuse_color;
    Color specular_color;
    int phong_exponent;
};

#endif // MATERIAL_H
