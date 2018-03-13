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
                 float bezier_resolution,
                 float point_size);

        ~Renderer();

        int create_application();

    private:
        unsigned int m_width;
        unsigned int m_height;
        float m_speed_factor;
        float m_bezier_resolution;
        float m_point_size;
        float m_time;

        std::string load_source(const std::string& filename) const;
        void compute_and_load_bezier_points();
        void init();
        void load_texture(std::unique_ptr<OpenGP::RGBA8Texture>& texture,
                          const std::string& filename) const;
        void draw_textures();
        void update();
    };
}

#endif // RENDERER_H
