#include "OpenGP/Image/Image.h"
#include "Ray.h"
#include "Surface.h"

using namespace OpenGP;

class Sphere : public Surface {
public:
    Vec3 position;
    float radius;
    Sphere(Vec3 p, float r);
    Vec3 hit(Ray ray);
};
