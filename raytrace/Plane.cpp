#include "Plane.h"

using namespace OpenGP;

Plane::Plane(Vec3 p, Vec3 n, Material m)  {
    position = p;
    normal = n;
    material = m;
}

float Plane::hit(Ray ray) {
    float t = -1;
    float denominator = ray.direction.dot(this->normal);
    if (abs(denominator) > 0.0001f) {
        float newT = (this->position - ray.origin).dot(this->normal) / denominator;
        if (newT >= 0) {
            t = newT;
        }
    }
    return t;
}
