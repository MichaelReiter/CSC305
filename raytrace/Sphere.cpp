#include "Sphere.h"

using namespace OpenGP;

Sphere::~Sphere() {}

Vec3 Sphere::getNormalAtPoint(Vec3 point) {
    return (point - this->position).normalized();
}

float Sphere::hit(Ray ray) {
    Vec3 e = ray.origin();
    Vec3 d = ray.direction();

    Vec3 ec = e - this->position;
    float discriminant = std::powf(ray.direction().dot(ec), 2) - ec.dot(ec) + this->radius*this->radius;

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
