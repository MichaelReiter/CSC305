#version 330 core

uniform vec3 camera_position;
uniform vec3 light_direction;
uniform vec3 light_color;
uniform vec3 ambient_coefficient;
uniform vec3 object_ambient;
uniform vec3 object_diffuse;
uniform vec3 object_specular;
uniform float phong_exponent;
// uniform sampler2D texture_image;

in vec3 fragment_position;
in vec3 fragment_normal;
// in vec2 fragment_texture;

out vec4 fragment_color;

void main() {
    vec3 normal = normalize(fragment_normal);
    vec3 view = normalize(camera_position - fragment_position);
    float diffuse_coefficient = max(0, -dot(light_direction, normal));
    float specular_coefficient = pow(max(0, dot(reflect(view, normal), light_direction)), phong_exponent);
    vec3 phong_lighting = ambient_coefficient * object_ambient
                        + diffuse_coefficient * light_color * object_diffuse
                        + specular_coefficient * light_color * object_specular;
    fragment_color = vec4(phong_lighting, 1);
    // fragment_color = texture(texture_image, fragment_texture).rgba;
}
