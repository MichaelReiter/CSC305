#include "OpenGP/Image/Image.h"
#include "Ray.h"

using namespace OpenGP;

class Surface {
public:
    Vec3 position;
    virtual Vec3 hit(Ray ray) = 0;
};
