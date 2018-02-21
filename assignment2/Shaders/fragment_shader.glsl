#version 330 core

uniform vec3 camera_position;
uniform vec3 light_direction;
uniform vec3 light_color;
uniform vec3 ambient_coefficient;
uniform vec3 object_ambient;
uniform vec3 object_diffuse;
uniform vec3 object_specular;
uniform float phong_exponent;

in vec3 fposition;
in vec3 fnormal;

out vec4 fcolor;

void main() {
    vec3 normal = normalize(fnormal);
    vec3 view = normalize(camera_position - fposition);
    float diffuse_coefficient = max(-dot(light_direction, normal), 0);
    float specular_coefficient = pow(max(dot(reflect(view, normal), light_direction), 0), phong_exponent);
    vec3 phong_lighting = ambient_coefficient * object_ambient
                        + diffuse_coefficient * light_color * object_diffuse
                        + specular_coefficient * light_color * object_specular;
    fcolor = vec4(phong_lighting, 1);
    fcolor = vec4(1);
}
