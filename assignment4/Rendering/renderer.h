#ifndef RENDERER_H
#define RENDERER_H

#include <OpenGP/GL/Application.h>
#include <string>

namespace Rendering {
    class Renderer {
    public:
        Renderer(unsigned int width,
                 unsigned int height,
                 unsigned int restart_primitive);

        ~Renderer();

        int create_application();

    private:
        unsigned int m_width;
        unsigned int m_height;
        unsigned int m_restart_primitive;
        OpenGP::Vec3 m_camera_position;
        OpenGP::Vec3 m_camera_front;
        OpenGP::Vec3 m_camera_up;
        float m_halflife;
        float m_yaw;
        float m_pitch;

        std::string load_shader(const std::string& filename) const;
        void init();
        void generate_terrain_mesh();
        void generate_cube_mesh();
        void draw_skybox();
        void draw_terrain();
    };
}

#endif // RENDERER_H
