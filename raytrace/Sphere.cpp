#include "Sphere.h"

Sphere::Sphere(Vec3 p, float r, Material m) :
    Surface(p, m),
    radius(r)
{}

Sphere::~Sphere() {}

Vec3 Sphere::GetNormal(Vec3 point) {
    return (point - position).normalized();
}

float Sphere::GetRayIntersectionParameter(Ray ray) {
    Vec3 e = ray.origin();
    Vec3 d = ray.direction();

    Vec3 ec = e - position;
    float discriminant = std::powf(ray.direction().dot(ec), 2) - ec.dot(ec) + radius*radius;

    float t = -1;
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
