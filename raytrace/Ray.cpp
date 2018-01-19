#include "Ray.h"

using namespace OpenGP;

Ray::Ray(Vec3 o, Vec3 d) {
    origin = o;
    direction = d;
};

Vec3 Ray::position(double t) {
    return origin + t * direction;
}
