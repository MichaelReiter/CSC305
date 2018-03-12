#version 330 core
uniform int selection;

out vec4 color;

void main() {
    if (selection == gl_PrimitiveID) {
        color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    } else {
        color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}
