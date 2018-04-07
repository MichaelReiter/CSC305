#version 330 core

// The camera position
uniform vec3 viewPos;
uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D sand;
uniform sampler2D snow;
uniform sampler2D water;
uniform sampler2D water2;
uniform sampler2D noiseTex;
uniform float wave_motion;

// Fragment position in world space coordinates
in vec3 fragPos;
in vec2 uv;
in float water_level;

out vec4 color;

void main() {
    // Directional light source
    vec3 light_position = vec3(1.0f, 1.0f, 10.0f);

    // Texture size in pixels
    ivec2 size = textureSize(noiseTex, 0);

    vec2 distorted_texture_coordinates = texture(water, vec2(uv.x + wave_motion, uv.y)).rg * 0.1f;
	distorted_texture_coordinates = uv + vec2(distorted_texture_coordinates.x, distorted_texture_coordinates.y + wave_motion);

    // Calculate surface normal
    vec3 A = vec3(uv.x + 1.0f / size.x, uv.y, textureOffset(noiseTex, uv, ivec2(1, 0)));
    vec3 B = vec3(uv.x - 1.0f / size.x, uv.y, textureOffset(noiseTex, uv, ivec2(-1, 0)));
    vec3 C = vec3(uv.x, uv.y + 1.0f / size.y, textureOffset(noiseTex, uv, ivec2(0, 1)));
    vec3 D = vec3(uv.x, uv.y - 1.0f / size.y, textureOffset(noiseTex, uv, ivec2(0, -1)));
    vec3 normal = normalize(cross(normalize(A - B), normalize(C - D)));

    float denominator = 1.0f / size.x;
    float slope = 0.0f;
    slope = max(slope, (A.y - fragPos.y) / denominator);
    slope = max(slope, (B.y - fragPos.y) / denominator);
    slope = max(slope, (C.y - fragPos.y) / denominator);
    slope = max(slope, (D.y - fragPos.y) / denominator);

    float sand_level = water_level + 0.03f;
    float grass_level = sand_level + 0.07f;
    float snow_level = 0.7f;
    float sand_slope = 0.7f;
    float snow_slope = 0.85f;

    // Texture according to height and slope
    float height = (texture(noiseTex, uv).r + 1.0f) / 2.0f;
    vec4 c = texture(sand, uv);
    if (height < water_level) {
        c = texture(water2, distorted_texture_coordinates);
    }
    if (height > water_level && height < sand_level && slope < sand_slope) {
        c = texture(sand, uv);
    }
    if (height > sand_level && height < grass_level) {
        c = texture(grass, uv);
    }
    if (height > grass_level) {
        c = texture(rock, uv);
    }
    if (height > snow_level && slope < snow_slope) {
        c = texture(snow, uv);
    }

    // Calculate ambient, diffuse, and specular shading
    vec3 light_direction = normalize(light_position - fragPos);
    float ambient = 0.05f;
    float diffuse_coefficient = 0.3f;
    float diffuse = diffuse_coefficient * max(0.0f, -dot(normal, light_direction));
    float specular_coefficient = 0.2f;
    float phong_exponent = 64.0f;
    vec3 view_direction = viewPos - fragPos;
    vec3 halfway = normalize(light_direction + view_direction);
    float specular = specular_coefficient * max(0.0f, pow(dot(normal, halfway), phong_exponent));

    c += (ambient + diffuse + specular);

    color = vec4(c);
}
