#include "Light.h"

using namespace OpenGP;

Light::Light(Vec3 p, double i) {
    position = p;
    intensity = i;
}

Light::~Light() {}

Vec3 Light::getPosition() {
    return position;
}

float Light::getIntensity() {
    return intensity;
}
