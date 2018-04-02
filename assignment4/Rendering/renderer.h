#ifndef RENDERER_H
#define RENDERER_H

#include <OpenGP/GL/Application.h>
#include <string>

namespace Rendering {
    class Renderer {
    public:
        Renderer(unsigned int width,
                 unsigned int height,
                 float field_of_view,
                 float movement_speed);

        ~Renderer();

        int create_application();

    private:
        unsigned int m_width;
        unsigned int m_height;
        unsigned int m_restart_primitive;
        float m_field_of_view;
        float m_movement_speed;
        OpenGP::Vec3 m_camera_position;
        OpenGP::Vec3 m_camera_front;
        OpenGP::Vec3 m_camera_up;
        float m_halflife;
        float m_yaw;
        float m_pitch;

        std::string load_shader(const std::string& filename) const;
        void init();
        void generate_terrain_mesh(int resolution, float side_length);
        void generate_cube_mesh();
        void draw_skybox();
        void draw_terrain();
    };
}

#endif // RENDERER_H
