#include "plane.h"

Plane::Plane(Vec3 p, Vec3 n, Color c, int phong, bool t) :
    Surface(p, c, phong, t),
    normal(n)
{}

Plane::~Plane() {}

Vec3 Plane::get_normal(Vec3 point)
{
    return normal.normalized();
}

float Plane::get_ray_intersection_parameter(Ray ray)
{
    float t = -1;
    float denominator = ray.direction().dot(normal);
    if (abs(denominator) > 0.0001f) {
        float t_prime = (position - ray.origin()).dot(normal) / denominator;
        if (t_prime >= 0) {
            t = t_prime;
        }
    }
    return t;
}
