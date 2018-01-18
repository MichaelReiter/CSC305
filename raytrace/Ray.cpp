//
// Created by Michael Reiter on 2018-01-18.
//

#include "Ray.h"

using namespace OpenGP;

Ray::Ray(Vec3 o, Vec3 d) {
    origin = o;
    direction = d;
};

Vec3 Ray::position(double t) {
    // origin + direction * t
    return Vec3(0.0f, 0.0f, 0.0f);
}
