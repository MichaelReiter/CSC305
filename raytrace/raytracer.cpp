#include "colors.h"
#include "plane.h"
#include "raytracer.h"
#include "sphere.h"
#include <vector>

Raytracer::Raytracer(int w, int h, int s) :
    m_width_resolution(w),
    m_height_resolution(h),
    m_supersample_factor(s)
{}

Raytracer::~Raytracer() {}

// Phong lighting model: total color = ambient + diffuse + specular
Color Raytracer::phong_lighting(
    const std::vector<Light>& lights,
    const Vec3& intersection_point,
    const Vec3& camera_position,
    const Surface& s,
    const std::vector<Surface*>& scene
) const
{
    const float ambient_light_intensity = 0.25f;
    const float epsilon = 0.0001f;

    // Ambient component
    Color result = s.get_ambient_color(intersection_point) * ambient_light_intensity;

    for (auto const& light : lights) {
        // Cast ray toward lights to compute shadows 
        Vec3 light_direction = (light.get_position() - intersection_point).normalized();
        Vec3 adjusted_point = intersection_point + epsilon * light_direction;
        Ray shadow_ray = {adjusted_point, light_direction};

        float t = -1.0f;
        bool shadow = false;
        for (auto const& s : scene) {
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

    return Colors::clamp_color(result);
}

Image Raytracer::trace_rays() const
{
    int width = m_width_resolution * m_supersample_factor;
    int height = m_height_resolution * m_supersample_factor;
    float aspect_ratio = float(width) / float(height);
    Image image(height, width);

    // Boundaries
    float left = -1.0f * aspect_ratio;
    float right = 1.0f * aspect_ratio;
    float top = 1.0f;
    float bottom = -1.0f;

    // World space axes
    const Vec3 u = {1.0f, 0.0f, 0.0f};
    const Vec3 v = {0.0f, 1.0f, 0.0f};
    const Vec3 w = {0.0f, 0.0f, -1.0f};

    // Camera position
    const float focal_length = 15.0f;
    const Vec3 e = -focal_length * w;

    // All objects in scene
    Sphere sphere = {Vec3(2.0f, 0.0f, -40.0f), 1.0f, Colors::red, 32};
    Sphere sphere2 = {Vec3(-2.0f, 0.0f, -40.0f), 1.0f, Colors::blue, 256};
    Sphere sphere3 = {Vec3(0.0f, 0.0f, -30.0f), 1.0f, Colors::white, 32};
    Plane floor_plane = {Vec3(0.0f, -1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Colors::grey, 32, true};

    std::vector<Surface*> scene{
        &sphere,
        &sphere2,
        &sphere3,
        &floor_plane
    };

    // Point lights
    std::vector<Light> lights{
        {Vec3(50.0f, 40.0f, 0.0f), 0.75f},
        {Vec3(0.0f, 4.0f, -60.0f), 0.75f}
    };

    // For each pixel
    for (int row = 0; row < image.rows(); row++) {
        for (int col = 0; col < image.cols(); col++) {
            // Cast ray
            Vec3 pixel_position = left * u + (col * (right - left) / image.cols()) * u;
            pixel_position += bottom * v + (row * (top - bottom) / image.rows()) * v;
            Vec3 ray_direction = (pixel_position - e).normalized();
            Ray ray = {e, ray_direction};

            // Compute intersection point of nearest surface in scene
            float t = -1.0f;
            Surface* intersected_surface;
            for (auto const& s : scene) {
                float t_prime = s->get_ray_intersection_parameter(ray);
                if (t == -1.0f || (t_prime > 0 && t_prime < t)) {
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
                image(row, col) = Colors::black;
            }
        }
    }

    // Antialiasing
    if (m_supersample_factor > 1) {
        image = apply_supersampling(image);
    }

    return image;
}

Image Raytracer::apply_supersampling(Image& image) const
{
    int width = m_width_resolution * m_supersample_factor;
    int height = m_height_resolution * m_supersample_factor;
    Image supersampled_image(height / m_supersample_factor, width / m_supersample_factor);
    for (int row = 0; row < image.rows(); row++) {
        for (int col = 0; col < image.cols(); col++) {
            Color c1 = image(row, col);
            Color c2 = image(std::min(row+1, width * m_supersample_factor), col);
            Color c3 = image(row, std::min(col+1, height * m_supersample_factor));
            Color c4 = image(std::min(row+1, width * m_supersample_factor), std::min(col+1, height * m_supersample_factor));
            Color averaged_color = (c1 + c2 + c3 + c4) / 4;
            supersampled_image(row / m_supersample_factor, col / m_supersample_factor) = averaged_color;
        }
    }
    return supersampled_image;
}
