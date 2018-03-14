#ifndef NOISE_H
#define NOISE_H

#include "OpenGP/GL/Application.h"

namespace Noise {
    OpenGP::R32FTexture* fBm2DTexture();
    float* perlin2D(const int width, const int height, const int period=64);
}

#endif // NOISE_H
