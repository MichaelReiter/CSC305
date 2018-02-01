#include "Plane.h"

Plane::Plane(Vec3 p, Vec3 n, Material m) :
    Surface(p, m),
    normal(n)
{}

Plane::~Plane() {}

Vec3 Plane::GetNormal(Vec3 point) {
    return normal.normalized();
}

float Plane::GetRayIntersectionParameter(Ray ray) {
    float t = -1;
    float denominator = ray.direction().dot(normal);
    if (abs(denominator) > 0.0001f) {
        float newT = (position - ray.origin()).dot(normal) / denominator;
        if (newT >= 0) {
            t = newT;
        }
    }
    return t;
}
