#include <cmath>
#include <cstdlib>
#include "noise.h"
#include "OpenGP/GL/Application.h"

namespace Noise {
    inline float lerp(float x, float y, float t)
    {
        return x + t * (y - x);
    }

    inline float fade(float t)
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    inline float rand01()
    {
        return ((float) std::rand()) / ((float)RAND_MAX);
    }

    // Generates a heightmap using fractional brownian motion
    OpenGP::R32FTexture* fBm2DTexture()
    {
        // Precompute perlin noise on a 2D grid
        const int width = 512;
        const int height = 512;
        float* perlin_data = perlin2D(width, height, 128);

        // fBm parameters
        float H = 0.8f;
        float lacunarity = 2.0f;
        float offset = 0.1f;
        const int octaves = 4;

        // Initialize to 0s
        float* noise_data = new float[width * height];
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                noise_data[i + j * height] = 0;
            }
        }

        // Precompute exponent array
        float* exponent_array = new float[octaves];
        float f = 1.0f;
        for (int i = 0; i < octaves; i++) {
            // TODO: Implement this step according to Musgraves paper on fBM

        }

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                int I = i;
                int J = j;
                for (int k = 0; k < octaves; k++) {
                    // TODO: Get perlin noise at I, J
                    // add offset, multiply by proper term and add to noise_data
                    noise_data[i + j * height] += 0;

                    // Point to sample at next octave
                    I *= (int)lacunarity;
                    J *= (int)lacunarity;
                }
            }
        }

        OpenGP::R32FTexture* _tex = new OpenGP::R32FTexture();
        _tex->upload_raw(width, height, noise_data);

        delete perlin_data;
        delete[] noise_data;
        delete[] exponent_array;

        return _tex;
    }

    float* perlin2D(const int width, const int height, const int period)
    {
        // Precompute random gradients
        float* gradients = new float[width * height * 2];
        auto sample_gradient = [&](int i, int j) {
            float x = gradients[2 * (i + j * height)];
            float y = gradients[2 * (i + j * height) + 1];
            return OpenGP::Vec2(x, y);
        };

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                float angle = rand01();
                gradients[2 * (i + j * height)] = cos(2 * angle * M_PI);
                gradients[2 * (i + j * height) + 1] = sin(2 * angle * M_PI);
            }
        }

        // Perlin Noise parameters
        float frequency = 1.0f / period;

        float* perlin_data = new float[width * height];
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {

                // Integer coordinates of corners
                int left = (i / period) * period;
                int right = (left + period) % width;
                int top = (j / period) * period;
                int bottom = (top + period) % height;

                // local coordinates [0, 1] within each block
                float dx = (i - left) * frequency;
                float dy = (j - top) * frequency;

                // Fetch random vectors at corners
                OpenGP::Vec2 top_left = sample_gradient(left, top);
                OpenGP::Vec2 top_right = sample_gradient(right, top);
                OpenGP::Vec2 bottom_left = sample_gradient(left, bottom);
                OpenGP::Vec2 bottom_right = sample_gradient(right, bottom);

                // Vector from each corner to pixel center
                OpenGP::Vec2 a(dx, -dy); // top_left
                OpenGP::Vec2 b(dx - 1, -dy); // top_right
                OpenGP::Vec2 c(dx, 1 - dy); // bottom_left
                OpenGP::Vec2 d(dx - 1, 1 - dy); // bottom_right

                // Get scalars at corners
                float s = a.dot(top_left);
                float t = b.dot(top_right);
                float u = c.dot(bottom_left);
                float v = d.dot(bottom_right);

                // Interpolate along x
                float st = lerp(s, t, fade(dx));
                float uv = lerp(u, v, fade(dx));

                // Interpolate along y
                float noise = lerp(st, uv, fade(dy));

                perlin_data[i + j * height] = noise;
            }
        }

        delete[] gradients;
        return perlin_data;
    }
}
