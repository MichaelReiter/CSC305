#include "mesh.h"

#include <cassert>
#include <iostream>
#include "OpenGP/GL/check_error_gl.h"
#include "OpenGP/GL/shader_helpers.h"
#include "OpenGP/SurfaceMesh/SurfaceMesh.h"
#include "OpenGP/external/LodePNG/lodepng.cpp"

void Mesh::init()
{
    // Compile the shaders
    m_pid = OpenGP::load_shaders(
        "/Users/michael/Dropbox/Programming/icg/build/assignment2/Mesh_vshader.glsl",
        "/Users/michael/Dropbox/Programming/icg/build/assignment2/Mesh_fshader.glsl"
    );
    if (!m_pid) {
        exit(EXIT_FAILURE);
    }
    check_error_gl();
    m_num_vertices = 0;
    m_has_normals = false;
    m_has_textures = false;
    m_has_texture_coords = false;
}

void Mesh::load_vertices(const std::vector<OpenGP::Vec3>& vertices,
                         const std::vector<unsigned int>& indices)
{
    // Vertex one vertex array
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    check_error_gl();

    // Vertex Buffer
    glGenBuffers(1, &m_vertex_point_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_point_buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(OpenGP::Vec3),
                 &vertices[0],
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3, // vec3
                          GL_FLOAT,
                          GL_FALSE, // DONT_NORMALIZE
                          3 * sizeof(float), // STRIDE
                          (void*)0); // ZERO_BUFFER_OFFSET
    check_error_gl();

    GLuint vbo_indices;
    glGenBuffers(1, &vbo_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 &indices[0],
                 GL_STATIC_DRAW);
    check_error_gl();
    m_num_vertices = (unsigned)indices.size();
    glBindVertexArray(0);
}

void Mesh::load_normals(const std::vector<OpenGP::Vec3>& normals)
{
    // Vertex one vertex Array
    glBindVertexArray(m_vao);
    check_error_gl();

    glGenBuffers(1, &m_vertex_normal_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_normal_buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 normals.size() * sizeof(OpenGP::Vec3),
                 &normals[0],
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3, // vec3
                          GL_FLOAT,
                          GL_TRUE, // NORMALIZE
                          3 * sizeof(float), // STRIDE
                          (void*)0); // ZERO_BUFFER_OFFSET
    check_error_gl();

    m_has_normals = true;
    glBindVertexArray(0);
}

void Mesh::load_texture_coordinates(const std::vector<OpenGP::Vec2>& texture_coordinates)
{
    // Vertex one vertex Array
    glBindVertexArray(m_vao);
    check_error_gl();

    glGenBuffers(1, &m_texture_coordinates_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_texture_coordinates_buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 texture_coordinates.size() * sizeof(OpenGP::Vec3),
                 &texture_coordinates[0],
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,
                          2, // vec2
                          GL_FLOAT,
                          GL_FALSE, // DONT_NORMALIZE
                          2 * sizeof(float), // STRIDE
                          (void*)0); // ZERO_BUFFER_OFFSET
    check_error_gl();

    m_has_texture_coords = true;
    glBindVertexArray(0);
}

void Mesh::load_textures(const std::string& filename)
{
    // Raw pixels
    std::vector<unsigned char> image;
    unsigned int width;
    unsigned int height;
    unsigned int error = lodepng::decode(image, width, height, filename);
    if (error) {
        std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    }
    // The pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA...
    // Use it as texture, draw it
    // Image is upside down, so correct that
    unsigned char* row = new unsigned char[4 * width];
    for (int i = 0; i < int(height) / 2; i++) {
        memcpy(row, &image[4 * i * width], 4 * width * sizeof(unsigned char));
        memcpy(&image[4 * i * width],
               &image[image.size() - 4 * (i + 1) * width],
               4 * width * sizeof(unsigned char));
        memcpy(&image[image.size() - 4 * (i + 1) * width], row, 4 * width * sizeof(unsigned char));
    }
    delete row;

    glBindVertexArray(m_vao);
    check_error_gl();

    glUseProgram(m_pid);

    GLuint texture_id = glGetUniformLocation(m_pid, "texImage");
    check_error_gl();
    glUniform1i(texture_id, 0);
    check_error_gl();

    glGenTextures(1, &m_texture_buffer);
    glBindTexture(GL_TEXTURE_2D, m_texture_buffer);

    check_error_gl();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    check_error_gl();

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 width,
                 height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 &image[0]);
    check_error_gl();

    m_has_textures = true;
    glUseProgram(0);
    glBindVertexArray(0);
}

void Mesh::draw(const OpenGP::Mat4x4& model, const OpenGP::Mat4x4& view, const OpenGP::Mat4x4& projection)
{
    if (!m_num_vertices) {
        return;
    }

    glUseProgram(m_pid);
    glBindVertexArray(m_vao);
    check_error_gl();

    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_point_buffer);

    // Use normals when shading
    if (m_has_normals) {
        glUniform1i(glGetUniformLocation(m_pid, "m_has_normals"), 1);
    } else {
        glUniform1i(glGetUniformLocation(m_pid, "m_has_normals"), 0);
    }

    // Use textures when shading
    if (m_has_textures && m_has_texture_coords) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture_buffer);
        glUniform1i(glGetUniformLocation(m_pid, "m_has_textures"), 1);
    } else {
        glUniform1i(glGetUniformLocation(m_pid, "m_has_textures"), 0);
    }

    // Set the MVP to vshader
    glUniformMatrix4fv(glGetUniformLocation(m_pid, "MODEL"), 1, GL_FALSE, model.data());
    glUniformMatrix4fv(glGetUniformLocation(m_pid, "VIEW"), 1, GL_FALSE, view.data());
    glUniformMatrix4fv(glGetUniformLocation(m_pid, "PROJ"), 1, GL_FALSE, projection.data());

    check_error_gl();
    // Draw
    glDrawElements(GL_TRIANGLES,
                   m_num_vertices,
                   GL_UNSIGNED_INT,
                   0); // ZERO_BUFFER_OFFSET
    check_error_gl();

    // Clean up
    glBindVertexArray(0);
    glUseProgram(0);
}
