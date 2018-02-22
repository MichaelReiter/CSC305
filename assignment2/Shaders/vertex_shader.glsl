#version 330 core

uniform mat4 model;
uniform mat4 model_rotation;
uniform mat4 MVP;

in vec3 vposition;
in vec3 vnormal;

out vec3 fragment_position;
out vec3 fragment_normal;

void main() {
    gl_Position = MVP * vec4(vposition, 1);
    fragment_position = (model * vec4(vposition, 1)).xyz;
    // fragment_normal = (model_rotation * vec4(vnormal, 1)).xyz;
}
