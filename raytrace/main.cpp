#include "OpenGP/Image/Image.h"
#include "bmpwrite.h"
#include "Light.h"
#include "Material.h"
#include "Plane.h"
#include "Sphere.h"
#include <vector>

using namespace OpenGP;
using Colour = Vec3; // RGB Value
using Ray = Eigen::ParametrizedLine<float, 3>;

// typedef Eigen::ParametrizedLine<float, 3> Ray;

Colour red() { return Colour(1.0f, 0.0f, 0.0f); }
Colour blue() { return Colour(0.0f, 0.0f, 1.0f); }
Colour white() { return Colour(1.0f, 1.0f, 1.0f); }
Colour black() { return Colour(0.0f, 0.0f, 0.0f); }
Colour lightGrey() { return Colour(0.75f, 0.75f, 0.75f); }
Colour grey() { return Colour(0.5f, 0.5f, 0.5f); }

Color ClampColor(Color c) {
    float r = std::fminf(c[0], 1);
    float g = std::fminf(c[1], 1);
    float b = std::fminf(c[2], 1);
    return Color(r, g, b);
}

void PrintColor(Color c) {
    printf("red: %f\ngreen: %f\nblue: %f\n", c[0], c[1], c[2]);
}

// Phong lighting model: total color = ambient + diffuse + specular
Color PhongLighting(std::vector<Light>& lights, Vec3 intersectionPoint, Vec3 cameraPosition, Surface& s) {
    const float kAmbientLightIntensity = 0.25f;
    const float kEpsilon = 0.0001f;

    // Ambient color = ambient material coefficient (surface color) * ambient light source
    Color result = s.material.ambientColor * kAmbientLightIntensity;

    for (auto light : lights) {
        // Shadows
        Vec3 lightDirection = (light.position - intersectionPoint).normalized();
        Vec3 adjustedPoint = intersectionPoint + kEpsilon * lightDirection;
        Ray ray = Ray(adjustedPoint, lightDirection);

        bool shadow = false;
        // https://stackoverflow.com/questions/19501838/get-derived-type-via-base-class-virtual-function

        // If no occulusion (no collision), then add diffuse and specular components to light
        if (!shadow) {
            // Diffuse color = diffuse material coefficient * (incoming light ray dotted with surface normal) * light source
            Vec3 normal = s.getNormalAtPoint(intersectionPoint);
            Color diffuseColor = light.intensity * std::fmaxf(0.0f, normal.dot(lightDirection)) * s.material.diffuseColor;
            result += diffuseColor;

            // Specular color = specular material coefficient * (normal dotted with h) to the power of phongExponent * light source
            Vec3 v = (cameraPosition - intersectionPoint).normalized();
            Vec3 h = (v + lightDirection).normalized();
            Color specularColor = light.intensity * std::powf(std::fmaxf(0.0f, normal.dot(h)), s.material.phongExponent) * s.material.specularColor;
            result += specularColor;
        }
    }

    return ClampColor(result);
}

int main(int argc, char** argv) {
    // number of columns (nx)
    const int kWidthResolution = 640;
    // number of rows (ny)
    const int kHeightResolution = 480;

    float aspectRatio = float(kWidthResolution) / float(kHeightResolution);
    Image<Colour> image(kHeightResolution, kWidthResolution);

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
    Vec3 spherePosition = Vec3(0.0f, 0.0f, -40.0f);
    float sphereRadius = 1.0f;
    Material sphereMaterial = Material(red(), red(), grey(), 32);
    Sphere sphere = Sphere(spherePosition, sphereRadius, sphereMaterial);

    // Floor plane
    Vec3 floorPosition = Vec3(0.0f, -1.0f, 0.0f);
    Vec3 floorNormal = Vec3(0.0f, 1.0f, 0.0f);
    Material floorMaterial = Material(blue(), blue(), grey(), 32);
    Plane floorPlane = Plane(floorPosition, floorNormal, floorMaterial);

    // All objects in scene
    std::vector<Surface*> scene;
    scene.push_back(&sphere);
    scene.push_back(&floorPlane);

    // Point lights
    Light light = Light(Vec3(50.0f, 40.0f, 0.0f), 0.75f);
    Light light2 = Light(Vec3(0.0f, 4.0f, -40.0f), 0.75f);
    std::vector<Light> lights;
    lights.push_back(light);
    // lights.push_back(light2);

    // For each pixel
    for (int row = 0; row < image.rows(); row++) {
        for (int col = 0; col < image.cols(); col++) {
            // Cast ray
            Vec3 pixelPosition = left * U + (col * (right - left) / image.cols()) * U;
            pixelPosition += bottom * V + (row * (top - bottom) / image.rows()) * V;
            Vec3 rayDirection = (pixelPosition - e).normalized();
            Ray ray = Ray(e, rayDirection);

            // Compute intersection point of object in scene
            float t = -1;
            Surface* intersectedSurface;
            for (auto s : scene) {
                float newT = s->hit(ray);
                if (t == -1) {
                    t = newT;
                    intersectedSurface = s;
                }
                if (newT > 0 && newT < t) {
                    t = newT;
                    intersectedSurface = s;
                }
            }
            Vec3 intersectionPoint = ray.pointAt(t);

            // Color pixels
            if (t > 0) {
                image(row, col) = PhongLighting(lights, intersectionPoint, e, *intersectedSurface);
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
