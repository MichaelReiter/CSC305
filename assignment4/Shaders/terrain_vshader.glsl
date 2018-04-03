#version 330 core
uniform sampler2D noiseTex;

in vec3 vposition;
in vec2 vtexcoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 uv;
out vec3 fragPos;
out float water_level;

void main() {
    uv = vtexcoord;

    float water_z = 0.45f;

    // Sample height from texture at uv
    float height = (texture(noiseTex, uv).r + 1.0f) / 2.0f;
    height = max(height, water_z);

    fragPos = vposition.xyz + vec3(0.0f, 0.0f, height);
    gl_Position = P * V * M * vec4(vposition.x, vposition.y, vposition.z + 10 * height, 1.0f);
    water_level = water_z;
}
