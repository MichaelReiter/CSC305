#ifndef COLORS_H
#define COLORS_H

#include "OpenGP/Image/Image.h"

using namespace OpenGP;
using Color = Vec3;

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
