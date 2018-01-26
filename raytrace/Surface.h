#ifndef SURFACE_H
#define SURFACE_H

#include "OpenGP/Image/Image.h"
#include "Material.h"
#include "Ray.h"

using namespace OpenGP;

class Surface {
public:
    Vec3 position;
    Material material;
    virtual Vec3 hit(Ray ray) = 0;
};

#endif
