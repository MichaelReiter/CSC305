#ifndef SURFACE_H
#define SURFACE_H

#include "OpenGP/Image/Image.h"
#include "material.h"

using namespace OpenGP;
using Ray = Eigen::ParametrizedLine<float, 3>;

class Surface {
public:
    Surface() {}

    virtual ~Surface() {}

    virtual float get_ray_intersection_parameter(Ray ray) = 0;
    virtual Vec3 get_normal(Vec3 point) = 0;
    virtual Material get_material()
    {
        return material;
    }

protected:
    Vec3 position;
    Surface(Vec3 p, Material m) :
        position(p),
        material(m)
    {}

private:
    Material material;
};

#endif // SURFACE_H
