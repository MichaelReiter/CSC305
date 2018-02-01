#ifndef MATERIAL_H
#define MATERIAL_H

#include "OpenGP/Image/Image.h"

using namespace OpenGP;
using Color = Vec3;

class Material {
public:
    Material();
    Material(Color a, Color d, Color s, int p);

    ~Material();

    Color get_ambient_color();
    Color get_diffuse_color();
    Color get_specular_color();
    int get_phong_exponent();

private:
    Color ambient_color;
    Color diffuse_color;
    Color specular_color;
    int phong_exponent;
};

#endif // MATERIAL_H
