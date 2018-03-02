#ifndef RENDERER_H
#define RENDERER_H

#include <OpenGP/GL/Application.h>
#include <string>

namespace Rendering {
    typedef Eigen::Transform<float, 3, Eigen::Affine> Transform;

    class Renderer {
    public:
        Renderer(unsigned int width, unsigned int height, float speed_factor);

        ~Renderer();

        int create_application();

    private:
        unsigned int m_width;
        unsigned int m_height;
        float m_speed_factor;

        std::string load_source(const char* filename);
        void load_texture(std::unique_ptr<OpenGP::RGBA8Texture>& texture, const std::string& filename);
        void init();
        void draw_scene();
    };
}

#endif // RENDERER_H
