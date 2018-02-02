#include "sphere.h"

Sphere::Sphere(const Vec3& p, float r, const Color& c, int phong, bool t) :
    Surface(p, c, phong, t),
    m_radius(r)
{}

Sphere::~Sphere() {}

Vec3 Sphere::get_normal(const Vec3& point) const
{
    return (point - m_position).normalized();
}

float Sphere::get_ray_intersection_parameter(const Ray& ray) const
{
    Vec3 e = ray.origin();
    Vec3 d = ray.direction();

    Vec3 ec = e - m_position;
    float discriminant = std::powf(ray.direction().dot(ec), 2) - ec.dot(ec) + m_radius * m_radius;

    float t = -1.0f;
    if (discriminant >= 0) {
        float t0 = -ray.direction().dot(ec) - std::sqrtf(discriminant);
        float t1 = -ray.direction().dot(ec) + std::sqrtf(discriminant);
        if (t1 < t0) {
            t0 = t1;
        }
        t = t0;
    }

    return t;
}
