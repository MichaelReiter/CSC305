#version 330 core

// The camera position
uniform vec3 viewPos;
uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D sand;
uniform sampler2D snow;
uniform sampler2D water;
uniform sampler2D noiseTex;

// Fragment position in world space coordinates
in vec3 fragPos;
in vec2 uv;
in float water_level;

out vec4 color;

void main() {
    // Directional light source
    vec3 light_position = normalize(vec3(1.0f, 1.0f, 1.0f));

    // Texture size in pixels
    ivec2 size = textureSize(noiseTex, 0);

    // Calculate surface normal
    vec3 A = vec3(uv.x + 1.0f / size.x, uv.y, textureOffset(noiseTex, uv, ivec2(1, 0)));
    vec3 B = vec3(uv.x - 1.0f / size.x, uv.y, textureOffset(noiseTex, uv, ivec2(-1, 0)));
    vec3 C = vec3(uv.x, uv.y + 1.0f / size.y, textureOffset(noiseTex, uv, ivec2(0, 1)));
    vec3 D = vec3(uv.x, uv.y - 1.0f / size.y, textureOffset(noiseTex, uv, ivec2(0, -1)));
    vec3 normal = normalize(cross(normalize(A - B), normalize(C - D)));

    // Texture according to height and slope
    float height = (texture(noiseTex, uv).r + 1.0f) / 2.0f;
    vec4 c = texture(rock, uv);
    if (height < water_level) {
        c = texture(water, uv);
    }

    // Calculate ambient, diffuse, and specular shading
    vec3 light_direction = normalize(light_position - fragPos);
    float ambient = 0.15f;
    float diffuse_coefficient = 0.3f;
    float diffuse = diffuse_coefficient * max(0, -dot(normal, light_direction));
    float specular_coefficient = 0.15f;
    float phong_exponent = 32.0f;
    vec3 view_direction = viewPos - fragPos;
    vec3 halfway = normalize(light_direction + view_direction);
    float specular = specular_coefficient * max(0, pow(dot(normal, halfway), phong_exponent));

    c += (ambient + diffuse + specular);

    color = vec4(c);
}
