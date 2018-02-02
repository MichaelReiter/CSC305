#ifndef PLANE_H
#define PLANE_H

#include "OpenGP/Image/Image.h"
#include "surface.h"

using namespace OpenGP;
using Ray = Eigen::ParametrizedLine<float, 3>;

class Plane : public Surface {
public:
    Plane(const Vec3& p, const Vec3& n, const Color& c, int phong, bool t=false);

    ~Plane();

    float get_ray_intersection_parameter(const Ray& ray) const;
    Vec3 get_normal(const Vec3& point) const;

private:
    Vec3 normal;
};

#endif // PLANE_H
