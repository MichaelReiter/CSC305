#version 330 core
uniform int offsetID;

out vec4 color;

void main() {
    color = vec4(float(gl_PrimitiveID + offsetID) / 255, 0.0f, 0.0f, 1.0f);
}
