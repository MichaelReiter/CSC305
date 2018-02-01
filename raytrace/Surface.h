#ifndef SURFACE_H
#define SURFACE_H

#include "OpenGP/Image/Image.h"
#include "Material.h"

using namespace OpenGP;
using Ray = Eigen::ParametrizedLine<float, 3>;

class Surface {
public:
    Vec3 position;
    Material material;

    Surface() {}

    virtual ~Surface() {}

    virtual float GetRayIntersectionParameter(Ray ray) = 0;
    virtual Vec3 GetNormal(Vec3 point) = 0;

protected:
    Surface(Vec3 p, Material m) :
        position(p),
        material(m)
    {}
};

#endif // SURFACE_H
