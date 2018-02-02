#include "sphere.h"

Sphere::Sphere(Vec3 p, float r, Color c, int phong, bool t) :
    Surface(p, c, phong, t),
    radius(r)
{}

Sphere::~Sphere() {}

Vec3 Sphere::get_normal(Vec3 point)
{
    return (point - position).normalized();
}

float Sphere::get_ray_intersection_parameter(Ray ray)
{
    Vec3 e = ray.origin();
    Vec3 d = ray.direction();

    Vec3 ec = e - position;
    float discriminant = std::powf(ray.direction().dot(ec), 2) - ec.dot(ec) + radius * radius;

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

// Colour Sphere::texture_from_image(Vec3 point)
// {
//     float phi = acos((point.z() - position.z()) / radius);
//     float theta = atan2(point.y() - position.y(), point.x() - position.x());
//     float u = phi / (2 * M_PI);
//     float v = (M_PI - theta) / M_PI;
//     return this->image.at<Colour>((int)(image.cols*u), (int)(image.rows*v));
// }
