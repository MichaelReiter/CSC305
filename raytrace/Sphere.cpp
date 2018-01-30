#include "Sphere.h"

using namespace OpenGP;

Sphere::Sphere(Vec3 p, float r, Material m) {
    position = p;
    radius = r;
    material = m;
}

Vec3 Sphere::getNormalAtPoint(Vec3 point) {
    return (point - this->position).normalized();
}

float Sphere::hit(Ray ray) {
    Vec3 e = ray.origin;
    Vec3 d = ray.direction;

    Vec3 ec = e - this->position;
    float discriminant = std::powf(ray.direction.dot(ec), 2) - ec.dot(ec) + this->radius*this->radius;

    float t = -1;
    if (discriminant >= 0) {
        t = -ray.direction.dot(ec) - std::sqrtf(discriminant);
    }

    return t;

//    float discriminant = (d dot (e - c))^2 - (d dot d)*(((e - c) dot (e - c)) - R*R);
//
//    // TODO: be sure to check for floating point bugs
//
//    if (discriminant < 0) {
//        // If discriminant is negative, ray and sphere do not intersect
//    } else if (discriminant > 0) {
//        // If discriminant is positive, there are 2 points of intersection
//        // One as the ray enters the sphere, the other as the ray exits
//        float t1 = (-d * (e - c) + sqrt(discriminant)) / (d dot d);
//        float t2 = (-d * (e - c) - sqrt(discriminant)) / (d dot d);
//    } else {
//        // If discriminant is 0, the ray grazes the sphere,
//        // thus intersecting at exactly one point
//        float t = (-d * (e - c) / (d dot d);
//    }

    // return t;
}
