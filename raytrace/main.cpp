#include "OpenGP/Image/Image.h"
#include "bmpwrite.h"
#include "Light.h"
#include "Ray.h"
#include "Sphere.h"
#include <vector>

using namespace OpenGP;

using Colour = Vec3; // RGB Value
Colour red() { return Colour(1.0f, 0.0f, 0.0f); }
Colour white() { return Colour(1.0f, 1.0f, 1.0f); }
Colour black() { return Colour(0.0f, 0.0f, 0.0f); }

// REMINDER! normalize unit vectors
Color lighting(std::vector<Light> lights, Vec3 intersectionPoint, Sphere s) {
    // Phong lighting model
    // Total color = ambient + diffuse + specular

    // Ambient color = ambient material coefficient (surface color) * ambient light source
    // I_a = k_a * L_a
    Color ambientLightColor = Color(0.0f, 0.0f, 0.0f);
    float ambientLightIntensity = 1.0f;
    Color ambientColor = ambientLightColor * ambientLightIntensity;

    // For multiple point lights, L = I_a + sum of diffuse and specular color for each point light
    Color diffuseColor = black();
    Color specularColor = black();
    for (auto light : lights) {
        // Diffuse color = diffuse material coefficient * (incoming light ray dotted with surface normal) * light source
        // I_d = k_d * (l dot n) * L_p
        // l is a unit vector pointing from the surface to the light
        // l is computed by subtracting the intersection point of the ray and surface from the light source position
        // n is a unit vector pointing normal to the surface
        Vec3 normal = (intersectionPoint - s.position).normalized();
        Vec3 lightDirection = (light.position - intersectionPoint).normalized();
        diffuseColor += light.intensity * std::fmaxf(0.0f, normal.dot(lightDirection)) * red();

        // Specular color = specular material coefficient * (R dotted with V) to the power of some parameter alpha * light source
        // I_s = k_s * (R dot V)^alpha * L_p
        // v is a unit vector pointing from the surface to the camera
        // specularColor += 
    }

    return ambientColor + diffuseColor + specularColor;
}

int main(int, char**) {
    // number of columns (nx)
    int widthResolution = 800;
    // number of rows (ny)
    int heightResolution = 600;

    float aspectRatio = float(widthResolution) / float(heightResolution);
    Image<Colour> image(heightResolution, widthResolution);

    // Boundaries
    float left = -1.0f * aspectRatio;
    float right = 1.0f * aspectRatio;
    float top = 1.0f;
    float bottom = -1.0f;

    // World space axes
    Vec3 U = Vec3(1.0f, 0.0f, 0.0f);
    Vec3 V = Vec3(0.0f, 1.0f, 0.0f);
    Vec3 W = Vec3(0.0f, 0.0f, -1.0f);

    // Camera position
    float focalLength = 15.0f;
    Vec3 e = -focalLength * W;

    // Sphere
    Sphere sphere = Sphere(Vec3(0.0f, 0.0f, -40.0f), 1.0f);

    // Point lights
    std::vector<Light> lights {
        Light(Vec3(0.0f, 4.0f, 0.0f), 1.0f)
    };

    // TODO:
    // define floor plane

    // For each pixel
    for (int row = 0; row < image.rows(); row++) {
        for (int col = 0; col < image.cols(); col++) {

            Vec3 pixelPosition = left * U + (col * (right - left) / image.cols()) * U;
            pixelPosition += bottom * V + (row * (top - bottom) / image.rows()) * V;

            Vec3 rayDirection = (pixelPosition - e).normalized();
            Ray ray = Ray(e, rayDirection);

            // Vec3 ec = e - sphere.position;
            // float discriminant = std::powf(ray.direction.dot(ec), 2) - ec.dot(ec) + sphere.radius*sphere.radius;

            Vec3 intersectionPoint = sphere.hit(ray);
            if (intersectionPoint != Vec3(0,0,0)) {
                image(row, col) = lighting(lights, intersectionPoint, sphere);
            } else {
                // No intersection. Color pixel background color
                image(row, col) = white();
            }
        }
    }

    bmpwrite("./out.bmp", image);
    imshow(image);

    return EXIT_SUCCESS;
}
