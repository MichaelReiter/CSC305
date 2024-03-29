#ifndef MESH_H
#define MESH_H

#include "OpenGP/Image/Image.h"

namespace Rendering {
    class Mesh {
    public:
        Mesh();

        ~Mesh();

        void load_vertices(const std::vector<OpenGP::Vec3>& vertices,
                           const std::vector<unsigned int>& indices);
        void load_normals(const std::vector<OpenGP::Vec3>& normals);
        void load_texture_coordinates(const std::vector<OpenGP::Vec2>& texture_coordinates);
        void load_textures(const std::string& filename);
        void draw(unsigned int width, unsigned int height, float t);

    private:
        GLuint m_vao; // vertex array object
        GLuint m_pid; // GLSL shader program ID
        GLuint m_texture_buffer;
        GLuint m_vertex_point_buffer;
        GLuint m_vertex_normal_buffer;
        GLuint m_texture_coordinates_buffer;
        unsigned int m_num_vertices;
    };
}

#endif // MESH_H
