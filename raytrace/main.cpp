#include "OpenGP/Image/Image.h"
#include "bmpwrite.h"

using namespace OpenGP;

using Colour = Vec3; // RGB Value
Colour red() { return Colour(1.0f, 0.0f, 0.0f); }
Colour white() { return Colour(1.0f, 1.0f, 1.0f); }
Colour black() { return Colour(0.0f, 0.0f, 0.0f); }

// REMINDER! normalize unit vectors
Color lighting(Vec3 ray) {
    // Phong lighting model
    // Total intensity = ambient + diffuse + specular
    // L = (k_a * L_a) + (k_d * (l dot n) * L_p) + (k_s * (R dot V)^alpha * L_p)

    // Ambient intensity = ambient material coefficient (surface color) * ambient light source
    // I_a = k_a * L_a

    // Diffuse intensity = diffuse material coefficient * (incoming light ray dotted with surface normal) * light source
    // I_d = k_d * (l dot n) * L_p

    // l is a unit vector pointing from the surface to the light
    // l is computed by subtracting the intersection point of the ray and surface from the light source position
    // n is a unit vector pointing normal to the surface

    // Specular intensity = specular material coefficient * (R dotted with V) to the power of some parameter alpha * light source
    // I_s = k_s * (R dot V)^alpha * L_p

    // v is a unit vector pointing from the surface to the camera

    // for multiple point lights, L = I_a + sum of diffuse and specular intensity for each point light
    // return I_a + for_each_point_light(I_d + I_s)
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

//    double discriminant = (d dot (e - c))^2 - (d dot d)*(((e - c) dot (e - c)) - R*R);
//
//    // TODO: be sure to check for doubleing point bugs
//
//    if (discriminant < 0) {
//        // If discriminant is negative, ray and sphere do not intersect
//    } else if (discriminant > 0) {
//        // If discriminant is positive, there are 2 points of intersection
//        // One as the ray enters the sphere, the other as the ray exits
//        double t1 = (-d * (e - c) + sqrt(discriminant)) / (d dot d);
//        double t2 = (-d * (e - c) - sqrt(discriminant)) / (d dot d);
//    } else {
//        // If discriminant is 0, the ray grazes the sphere,
//        // thus intersecting at exactly one point
//        double t = (-d * (e - c) / (d dot d);
//    }

    // return t;
}

int main(int, char**) {
    // number of columns (nx)
    int widthResolution = 800;
    // number of rows (ny)
    int heightResolution = 600;

    double aspectRatio = double(widthResolution) / double(heightResolution);
    Image<Colour> image(heightResolution, widthResolution);

    // Boundaries
    double left = -1.0f * aspectRatio;
    double right = 1.0f * aspectRatio;
    double top = 1.0f;
    double bottom = -1.0f;

    // World space axes
    Vec3 U = Vec3(1.0f, 0.0f, 0.0f);
    Vec3 V = Vec3(0.0f, 1.0f, 0.0f);
    Vec3 W = Vec3(0.0f, 0.0f, -1.0f);

    // Camera position
    double focalLength = 15.0f;
    Vec3 E = -focalLength * W;

    // Sphere
    Vec3 spherePosition = Vec3(0.0f, 0.0f, -40.0f);
    double sphereRadius = 1.0f;

    // Point light
    Vec3 lightPosition = Vec3(0.0f, 4.0f, 0.0f);
    double lightIntensity = 1.0f;

    // TODO:
    // define floor plane

    // For each pixel
    for (int row = 0; row < image.rows(); row++) {
        for (int col = 0; col < image.cols(); col++) {

            Vec3 pixel = left * U + (col * (right - left) / image.cols()) * U;
            pixel += bottom * V + (row * (top - bottom) / image.rows()) * V;

            Vec3 ray = pixel - E;
            ray = ray.normalized();

            Vec3 EsubC = E - spherePosition;
            double discriminant = std::powf(ray.dot(EsubC), 2) - EsubC.dot(EsubC) + sphereRadius*sphereRadius;
            if (discriminant >= 0) {

                double t = -ray.dot(EsubC) - std::sqrtf(discriminant);
                Vec3 pos = E + t * ray;
                Vec3 normal = (pos - spherePosition) / sphereRadius;
                Vec3 lightDir = lightPosition - pos;
                lightDir = lightDir.normalized();

                image(row, col) = lightIntensity * std::fmaxf(0.0f, normal.dot(lightDir)) * red();
            } else {
                image(row, col) = white();
            }

            // Ray ray = Ray();

            // Compute u and v
            // u = l + (r − l) * (i + 0.5) / nx
            // v = b + (t − b) * (j + 0.5) / ny

            // Compute ray direction
            // d is image plane distance (i.e. focal length)
            // ray direction = −d*w + u*u + v*v

//            if (ray intersects an object with 0 <= t < infinity) {
//                Compute n
//                Evaluate shading model and set pixel to that color
//            } else {
//                // Set pixel color to background color
//                image(row, col) = black();
//            }
        }
    }

    bmpwrite("./out.bmp", image);
    imshow(image);

    return EXIT_SUCCESS;
}
