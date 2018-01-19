#include "OpenGP/Image/Image.h"

using namespace OpenGP;

class Light {
public:
    Vec3 position;
    double intensity;
    Light(Vec3 p, double i);
};
