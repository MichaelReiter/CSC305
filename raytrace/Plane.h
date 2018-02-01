#ifndef PLANE_H
#define PLANE_H

#include "OpenGP/Image/Image.h"
#include "Material.h"
#include "Surface.h"

using namespace OpenGP;
using Ray = Eigen::ParametrizedLine<float, 3>;

class Plane : public Surface {
public:
    Plane(Vec3 p, Vec3 n, Material m);

    ~Plane();

    float GetRayIntersectionParameter(Ray ray);
    Vec3 GetNormal(Vec3 point);

private:
    Vec3 normal;
};

#endif // PLANE_H
