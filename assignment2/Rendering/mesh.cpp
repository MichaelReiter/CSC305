#include "mesh.h"

#include <iostream>
#include "OpenGP/GL/check_error_gl.h"
#include "OpenGP/GL/shader_helpers.h"
#include "OpenGP/SurfaceMesh/SurfaceMesh.h"
#include "OpenGP/external/LodePNG/lodepng.cpp"

namespace Rendering {
    Mesh::Mesh() :
        m_num_vertices(0)
    {
        // Compile the shaders
        m_pid = OpenGP::load_shaders(
            "/Users/michael/Dropbox/Programming/icg/assignment2/Shaders/vertex_shader.glsl",
            "/Users/michael/Dropbox/Programming/icg/assignment2/Shaders/fragment_shader.glsl"
        );
        if (!m_pid) {
            exit(EXIT_FAILURE);
        }
        check_error_gl();
    }

    Mesh::~Mesh() {}

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
                     texture_coordinates.size() * sizeof(OpenGP::Vec2),
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
        delete[] row;

        glBindVertexArray(m_vao);
        check_error_gl();

        glUseProgram(m_pid);

        GLuint texture_id = glGetUniformLocation(m_pid, "texture_image");
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

        glUseProgram(0);
        glBindVertexArray(0);
    }

    void Mesh::draw(unsigned int width, unsigned int height, float t)
    {
        if (!m_num_vertices) {
            return;
        }

        // glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render in wireframe mode
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glUseProgram(m_pid);
        glBindVertexArray(m_vao);
        check_error_gl();

        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_point_buffer);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture_buffer);

        // Create model, view and projection matrices
        OpenGP::Mat4x4 model = OpenGP::Mat4x4::Identity();
        OpenGP::Vec3 camera_position = {5 * cosf(t), 2, 5 * sinf(t)};
        OpenGP::Mat4x4 view = OpenGP::look_at(camera_position,
                                              OpenGP::Vec3(0.0f, 1.0f, 0.0f),
                                              OpenGP::Vec3(0.0f, 1.0f, 0.0f));
        float aspect_ratio = float(width) / float(height);
        OpenGP::Mat4x4 projection = OpenGP::perspective(60, aspect_ratio, 0.1f, 50);
        OpenGP::Mat4x4 MVP = projection * view * model;
        OpenGP::Mat4x4 model_rotation = model.inverse().transpose();

        // Set vertex shader uniforms
        glUniformMatrix4fv(glGetUniformLocation(m_pid, "model"), 1, GL_FALSE, model.data());
        glUniformMatrix4fv(glGetUniformLocation(m_pid, "model_rotation"), 1, GL_FALSE, model_rotation.data());
        glUniformMatrix4fv(glGetUniformLocation(m_pid, "MVP"), 1, GL_FALSE, MVP.data());

        // Lighting properties
        OpenGP::Vec3 light_direction = OpenGP::Vec3(cosf(2 * t), -1, sinf(2 * t)).normalized();
        OpenGP::Vec3 light_color = {1.0f, 1.0f, 1.0f};
        OpenGP::Vec3 ambient_coefficient = {0.25f, 0.25f, 0.25f};
        OpenGP::Vec3 object_ambient = {1.0f, 0.0f, 0.0f};
        OpenGP::Vec3 object_diffuse = {1.0f, 0.0f, 0.0f};
        OpenGP::Vec3 object_specular = {0.6f, 0.6f, 0.6f};
        float phong_exponent = 32.0f;
 
        // Set fragment shader uniforms
        glUniform3fv(glGetUniformLocation(m_pid, "camera_position"), 1, camera_position.data());
        glUniform3fv(glGetUniformLocation(m_pid, "light_direction"), 1, light_direction.data());
        glUniform3fv(glGetUniformLocation(m_pid, "light_color"), 1, light_color.data());
        glUniform3fv(glGetUniformLocation(m_pid, "ambient_coefficient"), 1, ambient_coefficient.data());
        glUniform3fv(glGetUniformLocation(m_pid, "object_ambient"), 1, object_ambient.data());
        glUniform3fv(glGetUniformLocation(m_pid, "object_diffuse"), 1, object_diffuse.data());
        glUniform3fv(glGetUniformLocation(m_pid, "object_specular"), 1, object_specular.data());
        glUniform1f(glGetUniformLocation(m_pid, "phong_exponent"), phong_exponent);
        check_error_gl();

        // Draw mesh
        glDrawElements(GL_TRIANGLES,
                       m_num_vertices,
                       GL_UNSIGNED_INT,
                       0); // ZERO_BUFFER_OFFSET
        check_error_gl();

        // Clean up
        glBindVertexArray(0);
        glUseProgram(0);
    }
}
