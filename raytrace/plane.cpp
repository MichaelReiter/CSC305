#include "plane.h"

Plane::Plane(const Vec3& p, const Vec3& n, const Color& c, int phong, bool t) :
    Surface(p, c, phong, t),
    m_normal(n)
{}

Plane::~Plane() {}

Vec3 Plane::get_normal(const Vec3& point) const
{
    return m_normal.normalized();
}

float Plane::get_ray_intersection_parameter(const Ray& ray) const
{
    float t = -1.0f;
    float denominator = ray.direction().dot(m_normal);
    if (abs(denominator) > 0.0001f) {
        float t_prime = (m_position - ray.origin()).dot(m_normal) / denominator;
        if (t_prime >= 0) {
            t = t_prime;
        }
    }
    return t;
}
