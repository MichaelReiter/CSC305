#ifndef MESH_H
#define MESH_H

#include "OpenGP/Image/Image.h"

class Mesh {
public:
    GLuint get_program_id() const;

    void init();
    void load_vertices(const std::vector<OpenGP::Vec3>& vertex_array,
                       const std::vector<unsigned int>& index_array);
    void load_normals(const std::vector<OpenGP::Vec3>& normal_array);
    void load_texture_coordinates(const std::vector<OpenGP::Vec2>& texture_coordinates);
    void load_textures(const std::string& filename);
    void draw(const OpenGP::Mat4x4& model, const OpenGP::Mat4x4& view, const OpenGP::Mat4x4& projection);

private:
    GLuint m_vao; // vertex array object
    GLuint m_pid; // GLSL shader program ID
    GLuint m_texture_buffer;
    GLuint m_vertex_point_buffer;
    GLuint m_vertex_normal_buffer;
    GLuint m_texture_coordinates_buffer;
    unsigned int m_num_vertices;
    bool m_has_normals;
    bool m_has_textures;
    bool m_has_tex_coords;
};

#endif // MESH_H
