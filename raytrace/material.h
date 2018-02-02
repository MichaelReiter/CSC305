#ifndef MATERIAL_H
#define MATERIAL_H

#include "OpenGP/Image/Image.h"

using namespace OpenGP;
using Color = Vec3;

class Material {
public:
    Material();
    Material(Color c, int p, bool t=false);

    ~Material();

    Color get_ambient_color(Vec3 point);
    Color get_diffuse_color(Vec3 point);
    Color get_specular_color(Vec3 point);
    int get_phong_exponent();

    Color checkerboard_texture(Vec3 point);

private:
    Color ambient_color;
    Color diffuse_color;
    Color specular_color;
    int phong_exponent;
    bool checkerboard;
};

#endif // MATERIAL_H
