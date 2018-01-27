#include "OpenGP/Image/Image.h"
#include "bmpwrite.h"
#include "Light.h"
#include "Material.h"
#include "Plane.h"
#include "Ray.h"
#include "Sphere.h"
#include <vector>

using namespace OpenGP;

using Colour = Vec3; // RGB Value
Colour red() { return Colour(1.0f, 0.0f, 0.0f); }
Colour blue() { return Colour(0.0f, 0.0f, 1.0f); }
Colour white() { return Colour(1.0f, 1.0f, 1.0f); }
Colour black() { return Colour(0.0f, 0.0f, 0.0f); }
Colour lightGrey() { return Colour(0.75f, 0.75f, 0.75f); }
Colour grey() { return Colour(0.5f, 0.5f, 0.5f); }

// REMINDER! normalize unit vectors
Color lighting(std::vector<Light> &lights, Vec3 intersectionPoint, Vec3 cameraPosition, Surface const &s) {
    // Phong lighting model
    // Total color = ambient + diffuse + specular

    // Ambient color = ambient material coefficient (surface color) * ambient light source
    // I_a = k_a * L_a
    float ambientLightIntensity = 0.25f;
    Color ambientColor = s.material.ambientColor * ambientLightIntensity;

    // // Shadows
    // Vec3 occulusionPoint = sphere.hit(ray);
    // if (occulusionPoint != Vec3(0,0,0)) {

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
        diffuseColor += light.intensity * std::fmaxf(0.0f, normal.dot(lightDirection)) * s.material.diffuseColor;

        // Specular color = specular material coefficient * (normal dotted with h) to the power of phongExponent * light source
        // I_s = k_s * (n dot h)^phongExponent * L_p
        // v is a unit vector pointing from the surface to the camera
        Vec3 v = (cameraPosition - intersectionPoint).normalized();
        Vec3 h = (v + lightDirection).normalized();
        specularColor += light.intensity * std::powf(std::fmaxf(0.0f, normal.dot(h)), s.material.phongExponent) * s.material.specularColor;
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

    // Point lights
    std::vector<Light> lights {
        {Vec3(50.0f, 40.0f, 0.0f), 0.75f}
    };

    // Sphere
    Vec3 spherePosition = Vec3(0.0f, 0.0f, -40.0f);
    float sphereRadius = 3.0f;
    Material sphereMaterial = Material(red(), red(), grey(), 32);
    Sphere sphere = Sphere(spherePosition, sphereRadius, sphereMaterial);

    // Floor plane
    Vec3 floorPosition = Vec3(0.0f, -3.0f, 0.0f);
    Vec3 floorNormal = Vec3(0.0f, 1.0f, 0.0f);
    Material floorMaterial = Material(lightGrey(), lightGrey(), grey(), 1);
    Plane floorPlane = Plane(floorPosition, floorNormal, floorMaterial);

    // All objects in scene
    std::vector<Surface*> scene {
        &sphere,
        &floorPlane
    };

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
            Vec3 intersectionPoint = ray.position(t);

            // Color pixels
            if (t > 0) {
                image(row, col) = lighting(lights, intersectionPoint, e, *intersectedSurface);
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
