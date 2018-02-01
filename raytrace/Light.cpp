#include "Light.h"

using namespace OpenGP;

Light::Light(Vec3 p, double i) {
    position = p;
    intensity = i;
}

Light::~Light() {}

Vec3 Light::GetPosition() {
    return position;
}

float Light::GetIntensity() {
    return intensity;
}
