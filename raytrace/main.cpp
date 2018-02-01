#include "OpenGP/Image/Image.h"
#include "bmpwrite.h"
#include "light.h"
#include "material.h"
#include "plane.h"
#include "sphere.h"
#include <vector>

using namespace OpenGP;
using Colour = Vec3;
using Ray = Eigen::ParametrizedLine<float, 3>;

Colour red() { return Colour(1.0f, 0.0f, 0.0f); }
Colour blue() { return Colour(0.0f, 0.0f, 1.0f); }
Colour white() { return Colour(1.0f, 1.0f, 1.0f); }
Colour black() { return Colour(0.0f, 0.0f, 0.0f); }
Colour light_grey() { return Colour(0.75f, 0.75f, 0.75f); }
Colour grey() { return Colour(0.5f, 0.5f, 0.5f); }

Color clamp_color(Color c)
{
    float r = std::fminf(c[0], 1);
    float g = std::fminf(c[1], 1);
    float b = std::fminf(c[2], 1);
    return Color(r, g, b);
}

void print_color(Color c)
{
    printf("red: %f\ngreen: %f\nblue: %f\n", c[0], c[1], c[2]);
}

// Phong lighting model: total color = ambient + diffuse + specular
Color phong_lighting(std::vector<Light>& lights, Vec3 intersection_point, Vec3 camera_position, Surface& s)
{
    const float ambient_light_intensity = 0.25f;
    const float epsilon = 0.0001f;

    // Ambient color = ambient material coefficient (surface color) * ambient light source
    Color result = s.get_material().get_ambient_color() * ambient_light_intensity;

    for (auto light : lights) {
        // Shadows
        Vec3 light_direction = (light.get_position() - intersection_point).normalized();
        Vec3 adjusted_point = intersection_point + epsilon * light_direction;
        Ray ray = Ray(adjusted_point, light_direction);

        bool shadow = false;
        // https://stackoverflow.com/questions/19501838/get-derived-type-via-base-class-virtual-function

        // If no occulusion (no collision), then add diffuse and specular components to light
        if (!shadow) {
            // Diffuse color = diffuse material coefficient * (incoming light ray dotted with surface normal) * light source
            Vec3 normal = s.get_normal(intersection_point);
            Color diffuse_color = light.get_intensity() * std::fmaxf(0.0f, normal.dot(light_direction)) * s.get_material().get_diffuse_color();
            result += diffuse_color;

            // Specular color = specular material coefficient * (normal dotted with h) to the power of phong_exponent * light source
            Vec3 v = (camera_position - intersection_point).normalized();
            Vec3 h = (v + light_direction).normalized();
            Color specular_color = light.get_intensity() * std::powf(std::fmaxf(0.0f, normal.dot(h)), s.get_material().get_phong_exponent()) * s.get_material().get_specular_color();
            result += specular_color;
        }
    }

    return clamp_color(result);
}

int main(int argc, char** argv)
{
    // number of columns (nx)
    const int width_resolution = 640;
    // number of rows (ny)
    const int height_resolution = 480;

    float aspect_ratio = float(width_resolution) / float(height_resolution);
    Image<Colour> image(height_resolution, width_resolution);

    // Boundaries
    float left = -1.0f * aspect_ratio;
    float right = 1.0f * aspect_ratio;
    float top = 1.0f;
    float bottom = -1.0f;

    // World space axes
    Vec3 U = Vec3(1.0f, 0.0f, 0.0f);
    Vec3 V = Vec3(0.0f, 1.0f, 0.0f);
    Vec3 W = Vec3(0.0f, 0.0f, -1.0f);

    // Camera position
    float focal_length = 15.0f;
    Vec3 e = -focal_length * W;

    // Sphere
    Vec3 sphere_position = Vec3(0.0f, 0.0f, -40.0f);
    float sphere_radius = 1.0f;
    Material sphere_material = Material(red(), red(), grey(), 32);
    Sphere sphere = Sphere(sphere_position, sphere_radius, sphere_material);

    // Floor plane
    Vec3 floor_position = Vec3(0.0f, -1.0f, 0.0f);
    Vec3 floor_normal = Vec3(0.0f, 1.0f, 0.0f);
    Material floor_material = Material(blue(), blue(), grey(), 32);
    Plane floor_plane = Plane(floor_position, floor_normal, floor_material);

    // All objects in scene
    std::vector<Surface*> scene;
    scene.push_back(&sphere);
    scene.push_back(&floor_plane);

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
            Vec3 pixel_position = left * U + (col * (right - left) / image.cols()) * U;
            pixel_position += bottom * V + (row * (top - bottom) / image.rows()) * V;
            Vec3 ray_direction = (pixel_position - e).normalized();
            Ray ray = Ray(e, ray_direction);

            // Compute intersection point of object in scene
            float t = -1;
            Surface* intersected_surface;
            for (auto s : scene) {
                float newT = s->get_ray_intersection_parameter(ray);
                if (t == -1) {
                    t = newT;
                    intersected_surface = s;
                }
                if (newT > 0 && newT < t) {
                    t = newT;
                    intersected_surface = s;
                }
            }
            Vec3 intersection_point = ray.pointAt(t);

            // Color pixels
            if (t > 0) {
                image(row, col) = phong_lighting(lights, intersection_point, e, *intersected_surface);
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
