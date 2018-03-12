#ifndef RENDERER_H
#define RENDERER_H

#include <OpenGP/GL/Application.h>
#include <string>

namespace Rendering {
    class Renderer {
    public:
        Renderer(unsigned int width,
                 unsigned int height,
                 float speed_factor,
                 float point_size);

        ~Renderer();

        int create_application();

    private:
        unsigned int m_width;
        unsigned int m_height;
        float m_speed_factor;
        float m_point_size;

        std::string load_source(const std::string& filename) const;
        void init();
        void load_texture(std::unique_ptr<OpenGP::RGBA8Texture>& texture,
                          const std::string& filename) const;
        void draw_scene();
        void draw_path(OpenGP::Vec2* selection);
    };
}

#endif // RENDERER_H
