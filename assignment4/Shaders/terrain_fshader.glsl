#version 330 core
uniform sampler2D noiseTex;

// The camera position
uniform vec3 viewPos;
uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D sand;
uniform sampler2D snow;
uniform sampler2D water;

// Fragment position in world space coordinates
in vec3 fragPos;
in vec2 uv;

out vec4 color;

void main() {
    // Directional light source
    vec3 light_direction = normalize(vec3(1.0f, 1.0f, 1.0f));

    // Texture size in pixels
    ivec2 size = textureSize(noiseTex, 0);

    // TODO: Calculate surface normal N
    // HINT: Use textureOffset(,,) to read height at uv + pixelwise offset
    // HINT: Account for texture x,y dimensions in world space coordinates (default f_width=f_height=5)
    vec3 A = vec3(0);
    vec3 B = vec3(0);
    vec3 C = vec3(0);
    vec3 D = vec3(0);
    vec3 N = normalize(cross(normalize(A - B), normalize(C - D)));

    // TODO: Texture according to height and slope
    // HINT: Read noiseTex for height at uv

    // TODO: Calculate ambient, diffuse, and specular lighting
    // HINT: max(,) dot(,) reflect(,) normalize()

    color = vec4(0, 0, 0, 1);
}