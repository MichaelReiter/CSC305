#ifndef COLORS_H
#define COLORS_H

#include "OpenGP/Image/Image.h"

typedef OpenGP::Vec3 Vec3;
typedef Vec3 Color;

class Colors {
public:    
    static const Color red;
    static const Color blue;
    static const Color white;
    static const Color black;
    static const Color grey;

    static Color clamp_color(const Color& c);
};

#endif // COLORS_H
