#ifndef LOAD_TEXTURE_H
#define LOAD_TEXTURE_H

#include <OpenGP/GL/Application.h>
#include <vector>

namespace Texture {
    void load_texture(std::unique_ptr<OpenGP::RGBA8Texture>& texture, const char* filename);
    void load_texture(std::vector<unsigned char>& image, const char* filename);
}

#endif // LOAD_TEXTURE_H
