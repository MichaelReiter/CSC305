#ifndef SPHERE_H
#define SPHERE_H

#include "surface.h"

class Sphere : public Surface {
public:
    Sphere(const Vec3& p, float r, const Color& c, int phong, bool t=false);

    ~Sphere();

    float get_ray_intersection_parameter(const Ray& ray) const;
    Vec3 get_normal(const Vec3& point) const;

private:
    float m_radius;
};

#endif // SPHERE_H
