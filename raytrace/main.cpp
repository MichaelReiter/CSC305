#include "OpenGP/Image/Image.h"
#include "bmpwrite.h"

using namespace OpenGP;

using Colour = Vec3; // RGB Value
Colour red() { return Colour(1.0f, 0.0f, 0.0f); }
Colour white() { return Colour(1.0f, 1.0f, 1.0f); }
Colour black() { return Colour(0.0f, 0.0f, 0.0f); }

Color lighting(Vec3 ray) {
    // Phong lighting model
    // Total intensity = ambient + diffuse + specular
    // I = (k_a * L_a) + (k_d * (l dot n) * L_p) + (k_s * (R dot V)^alpha * L_p)

    // Ambient intensity = ambient material constant * ambient light source
    // I_a = k_a * L_a

    // Diffuse intensity = diffuse material constant * (incoming light ray dotted with surface normal) * light source
    // I_d = k_d * (l dot n) * L_p

    // Specular intensity = specular material constant * (R dotted with V) to the power of some parameter alpha * light source
    // I_s = k_s * (R dot V)^alpha * L_p

    // return I_a + I_d + I_s
    return Colour(0.0f, 0.0f, 0.0f);
}

Color trace(Vec3 ray) {
    // hit = find first intersection with scene objects
    Color color = lighting(ray);
    return color;
}

void ray_sphere_intersection(Vec3 ray) {
    // A ray is defined by the parametic equation p(t) = e + t*d
    // e is the camera origin point
    // d is the direction vector of the ray
    // t is a scalar magnitude parameter

    // A sphere is implicitly defined as
    // a point p being on a sphere of radius R centered at point c iff
    // ((p - c) dot (p - c)) - R^2 = 0

    double discriminant = (d dot (e - c))^2 - (d dot d)*(((e - c) dot (e - c)) - R*R);

    // TODO: be sure to check for floating point bugs

    if (discriminant < 0) {
        // If discriminant is negative, ray and sphere do not intersect
    } else if (discriminant > 0) {
        // If discriminant is positive, there are 2 points of intersection
        // One as the ray enters the sphere, the other as the ray exits
        double t1 = (-d * (e - c) + sqrt(discriminant)) / (d dot d);
        double t2 = (-d * (e - c) - sqrt(discriminant)) / (d dot d);
    } else {
        // If discriminant is 0, the ray grazes the sphere,
        // thus intersecting at exactly one point
        double t = (-d * (e - c) / (d dot d);
    }

    // return t;
}

int main(int, char**) {
    // number of columns (nx)
    int widthResolution = 800;

    // number of rows (ny)
    int heightResolution = 600;

    Image<Colour> image(heightResolution, widthResolution);

    // e
    Vec3 cameraPosition = Vec3(0, 0, 0);

    Vec3 spherePosition = Vec3(0, 5, 0);

    // TODO:
    // define floor plane
    // define a single point light source

    // For each pixel
    for (int row = 0; row < image.rows(); row++) {
        for (int col = 0; col < image.cols(); col++) {
            // Generate primary ray

            /// TODO: build primary rays
            // example: Vec3 o = Vec3(0,0,0);

            // Vec3 ray = Vec3(0,0,0);

            // Compute u and v
            // u = l + (r − l) * (i + 0.5) / nx
            // v = b + (t − b) * (j + 0.5) / ny

            // Compute ray direction
            // d is image plane distance (i.e. focal length)
            // ray direction = −d*w + u*u + v*v

            // ray origin is camera position (e)
            
            // Pixel color = trace(primary ray)

            /// TODO: ray sphere intersection and shading

            /// EXAMPLE: use "image(row, col) = colour;" to set pixel values
            // if (row > 0 && row < 200 && col > 0 && col < 200)  {
            //     image(row, col) = red();
            // } else {
            //     image(row, col) = white();
            // }
            image(row, col) = white();
        }
    }

    bmpwrite("./out.bmp", image);
    imshow(image);

    return EXIT_SUCCESS;
}
