#include "OpenGP/Image/Image.h"
#include "bmpwrite.h"
#include "light.h"
#include "plane.h"
#include "sphere.h"
#include <vector>

using namespace OpenGP;
using Color = Vec3;
using Ray = Eigen::ParametrizedLine<float, 3>;

Color red() { return Color(1.0f, 0.0f, 0.0f); }
Color blue() { return Color(0.0f, 0.0f, 1.0f); }
Color white() { return Color(1.0f, 1.0f, 1.0f); }
Color black() { return Color(0.0f, 0.0f, 0.0f); }
Color grey() { return Color(0.5f, 0.5f, 0.5f); }

// Ensure RGB values are constrained within [0, 1]
Color clamp_color(Color c)
{
    float r = std::fminf(c[0], 1);
    float g = std::fminf(c[1], 1);
    float b = std::fminf(c[2], 1);
    return Color(r, g, b);
}

// Phong lighting model: total color = ambient + diffuse + specular
Color phong_lighting(std::vector<Light>& lights, Vec3 intersection_point, Vec3 camera_position, Surface& s, std::vector<Surface*>& scene)
{
    const float ambient_light_intensity = 0.25f;
    const float epsilon = 0.0001f;

    // Ambient component
    Color result = s.get_ambient_color(intersection_point) * ambient_light_intensity;

    for (auto light : lights) {
        // Cast ray toward lights to compute shadows 
        Vec3 light_direction = (light.get_position() - intersection_point).normalized();
        Vec3 adjusted_point = intersection_point + epsilon * light_direction;
        Ray shadow_ray = Ray(adjusted_point, light_direction);

        float t = -1;
        bool shadow = false;
        for (auto s : scene) {
            if (dynamic_cast<Sphere*>(s)) {
                float t_prime = s->get_ray_intersection_parameter(shadow_ray);
                if (t_prime > 0) {
                    t = t_prime;
                }
                shadow = t > 0;
            }
        }

        // If no occulusion then add diffuse and specular components to lighting model
        if (!shadow) {
            // Diffuse component
            Vec3 normal = s.get_normal(intersection_point);
            Color diffuse_color = light.get_intensity() * std::fmaxf(0.0f, normal.dot(light_direction)) * s.get_diffuse_color(intersection_point);
            result += diffuse_color;

            // Specular component
            Vec3 v = (camera_position - intersection_point).normalized();
            Vec3 h = (v + light_direction).normalized();
            Color specular_color = light.get_intensity() * std::powf(std::fmaxf(0.0f, normal.dot(h)), s.get_phong_exponent()) * s.get_specular_color(intersection_point);
            result += specular_color;
        }
    }

    return clamp_color(result);
}

int main(int argc, char** argv)
{
    // Antialiased using supersampling
    const int supersample_factor = 2;
    // number of columns (nx)
    const int width_resolution = 1280 * supersample_factor;
    // number of rows (ny)
    const int height_resolution = 800 * supersample_factor;

    float aspect_ratio = float(width_resolution) / float(height_resolution);
    Image<Color> image(height_resolution, width_resolution);

    // Boundaries
    float left = -1.0f * aspect_ratio;
    float right = 1.0f * aspect_ratio;
    float top = 1.0f;
    float bottom = -1.0f;

    // World space axes
    const Vec3 u = Vec3(1.0f, 0.0f, 0.0f);
    const Vec3 v = Vec3(0.0f, 1.0f, 0.0f);
    const Vec3 w = Vec3(0.0f, 0.0f, -1.0f);

    // Camera position
    const float focal_length = 15.0f;
    const Vec3 e = -focal_length * w;

    // Spheres
    const Vec3 sphere_position = Vec3(2.0f, 0.0f, -40.0f);
    const float sphere_radius = 1.0f;
    Sphere sphere = Sphere(sphere_position, sphere_radius, red(), 32);

    const Vec3 sphere2_position = Vec3(-2.0f, 0.0f, -40.0f);
    const float sphere2_radius = 1.0f;
    Sphere sphere2 = Sphere(sphere2_position, sphere2_radius, blue(), 256);

    const Vec3 sphere3_position = Vec3(0.0f, 0.0f, -30.0f);
    const float sphere3_radius = 1.0f;
    Sphere sphere3 = Sphere(sphere3_position, sphere3_radius, white(), 32);

    // Floor plane
    const Vec3 floor_position = Vec3(0.0f, -1.0f, 0.0f);
    const Vec3 floor_normal = Vec3(0.0f, 1.0f, 0.0f);
    Plane floor_plane = Plane(floor_position, floor_normal, grey(), 32, true);

    // All objects in scene
    std::vector<Surface*> scene;
    scene.push_back(&sphere);
    scene.push_back(&sphere2);
    scene.push_back(&sphere3);
    scene.push_back(&floor_plane);

    // Point lights
    Light light = Light(Vec3(50.0f, 40.0f, 0.0f), 0.75f);
    Light light2 = Light(Vec3(0.0f, 4.0f, -60.0f), 0.75f);
    std::vector<Light> lights;
    lights.push_back(light);
    lights.push_back(light2);

    // For each pixel
    for (int row = 0; row < image.rows(); row++) {
        for (int col = 0; col < image.cols(); col++) {
            // Cast ray
            Vec3 pixel_position = left * u + (col * (right - left) / image.cols()) * u;
            pixel_position += bottom * v + (row * (top - bottom) / image.rows()) * v;
            Vec3 ray_direction = (pixel_position - e).normalized();
            Ray ray = Ray(e, ray_direction);

            // Compute intersection point of nearest surface in scene
            float t = -1;
            Surface* intersected_surface;
            for (auto s : scene) {
                float t_prime = s->get_ray_intersection_parameter(ray);
                if (t == -1 || (t_prime > 0 && t_prime < t)) {
                    t = t_prime;
                    intersected_surface = s;
                }
            }
            Vec3 intersection_point = ray.pointAt(t);

            // Color pixels
            if (t > 0) {
                image(row, col) = phong_lighting(lights, intersection_point, e, *intersected_surface, scene);
            } else {
                // No intersection. Default to background color
                image(row, col) = black();
            }
        }
    }

    // Antialiasing
    if (supersample_factor > 1) {
        Image<Color> supersampled_image(height_resolution / supersample_factor, width_resolution / supersample_factor);
        for (int row = 0; row < image.rows(); row++) {
            for (int col = 0; col < image.cols(); col++) {
                Color c1 = image(row, col);
                Color c2 = image(std::min(row+1, width_resolution * supersample_factor), col);
                Color c3 = image(row, std::min(col+1, height_resolution * supersample_factor));
                Color c4 = image(std::min(row+1, width_resolution * supersample_factor), std::min(col+1, height_resolution * supersample_factor));
                Color averaged_color = (c1 + c2 + c3 + c4) / 4;
                supersampled_image(row / supersample_factor, col / supersample_factor) = averaged_color;
            }
        }
        image = supersampled_image;
    }

    bmpwrite("./out.bmp", image);
    imshow(image);

    return EXIT_SUCCESS;
}
