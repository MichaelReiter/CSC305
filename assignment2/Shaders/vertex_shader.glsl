#version 330 core

uniform mat4 model;
uniform mat4 model_rotation;
uniform mat4 MVP;

in vec3 vertex_position;
in vec3 vertex_normal;
// in vec2 vertex_texture;

out vec3 fragment_position;
out vec3 fragment_normal;
// out vec2 fragment_texture;

void main() {
    gl_Position = MVP * vec4(vertex_position, 1);
    fragment_position = (model * vec4(vertex_position, 1)).xyz;
    fragment_normal = (model_rotation * vec4(vertex_normal, 1)).xyz;
    // fragment_texture = vertex_texture;
}
