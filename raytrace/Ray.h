#include "OpenGP/Image/Image.h"

using namespace OpenGP;

class Ray {
    Vec3 origin;
    Vec3 direction;
public:
    Ray(Vec3 o, Vec3 d);
    Vec3 position(double t);
};
