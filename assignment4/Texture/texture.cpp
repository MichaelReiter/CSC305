#include "texture.h"
#include <OpenGP/GL/Application.h>
#include <OpenGP/external/LodePNG/lodepng.cpp>

namespace Texture {
    void Texture::load_texture(std::unique_ptr<OpenGP::RGBA8Texture>& texture, const char* filename)
    {
        std::vector<unsigned char> image;
        unsigned int width;
        unsigned int height;
        unsigned int error = lodepng::decode(image, width, height, filename);
        if (error) {
            std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        }

        unsigned char* row = new unsigned char[4 * width];
        for (int i = 0; i < int(height) / 2; i++) {
            memcpy(row, &image[4 * i * width], 4 * width * sizeof(unsigned char));
            memcpy(&image[4 * i * width],
                &image[image.size() - 4 * (i + 1) * width],
                4 * width * sizeof(unsigned char));
            memcpy(&image[image.size() - 4 * (i + 1) * width],
                row,
                4 * width * sizeof(unsigned char));
        }
        delete[] row;

        texture = std::unique_ptr<OpenGP::RGBA8Texture>(new OpenGP::RGBA8Texture());
        texture->upload_raw(width, height, &image[0]);
    }

    void Texture::load_texture(std::vector<unsigned char>& image, const char* filename)
    {
        unsigned int width;
        unsigned int height;
        unsigned int error = lodepng::decode(image, width, height, filename);
        if (error) {
            std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        }

        unsigned char* row = new unsigned char[4 * width];
        for (int i = 0; i < int(height) / 2; i++) {
            memcpy(row, &image[4 * i * width], 4 * width * sizeof(unsigned char));
            memcpy(&image[4 * i * width],
                &image[image.size() - 4 * (i + 1) * width],
                4 * width * sizeof(unsigned char));
            memcpy(&image[image.size() - 4 * (i + 1) * width],
                row,
                4 * width * sizeof(unsigned char));
        }
        delete[] row;
    }
}
