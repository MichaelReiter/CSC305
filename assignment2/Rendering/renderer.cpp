#include "renderer.h"

#include <fstream>
#include <math.h>
#include <stdio.h>

namespace Rendering {
    Renderer::Renderer(unsigned int width, unsigned int height, const std::string& mesh_path) :
        m_width(width),
        m_height(height),
        m_mesh_path(mesh_path),
        m_time(0.0f)
    {}

    Renderer::~Renderer() {}

    void Renderer::update()
    {
        m_time += 0.001f;
    }

    std::string Renderer::read_file_to_string(const std::string& filename) const
    {
        std::ifstream file(filename);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    void Renderer::draw(OpenGP::Shader& shader, Mesh& mesh) const
    {
        glViewport(0, 0, m_width, m_height);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.bind();

        // Create model, view and projection matrices
        OpenGP::Mat4x4 model = OpenGP::Mat4x4::Identity();
        OpenGP::Vec3 camera_position = {5 * cosf(m_time), 2, 5 * sinf(m_time)};
        OpenGP::Mat4x4 view = OpenGP::look_at(camera_position,
                                            OpenGP::Vec3(0.0f, 1.0f, 0.0f),
                                            OpenGP::Vec3(0.0f, 1.0f, 0.0f));
        float aspect_ratio = float(m_width) / float(m_height);
        OpenGP::Mat4x4 projection = OpenGP::perspective(60, aspect_ratio, 0.1f, 50);
        OpenGP::Mat4x4 MVP = projection * view * model;

        // Apply any rotation from model matrix to vertex normals ignoring translation and scaling
        OpenGP::Mat4x4 model_rotation = model.inverse().transpose();

        // Pass uniforms to shaders
        shader.set_uniform("model", model);
        shader.set_uniform("model_rotation", model_rotation);
        shader.set_uniform("MVP", MVP);
        shader.set_uniform("camera_position", camera_position);

        // Set material and lighting parameters
        OpenGP::Vec3 light_direction = OpenGP::Vec3(cosf(2 * m_time), -1, sinf(2 * m_time)).normalized();
        shader.set_uniform("light_direction", light_direction);
        shader.set_uniform("light_color", OpenGP::Vec3(1.0f, 1.0f, 1.0f));
        shader.set_uniform("ambient_coefficient", OpenGP::Vec3(0.25f, 0.25f, 0.25f));
        shader.set_uniform("object_ambient", OpenGP::Vec3(1.0f, 0.0f, 0.0f));
        shader.set_uniform("object_diffuse", OpenGP::Vec3(1.0f, 0.0f, 0.0f));
        shader.set_uniform("object_specular", OpenGP::Vec3(0.6f, 0.6f, 0.6f));
        shader.set_uniform("phong_exponent", 32.0f);

        // mesh.set_attributes(shader);
        mesh.draw(model, view, projection);

        shader.unbind();
    }

    void Renderer::read_vertices_and_indices_from_file(const std::string& filename,
                                                    std::vector<OpenGP::Vec3>& vertices,
                                                    std::vector<unsigned int>& indices) const
    {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                char c;
                float x;
                float y;
                float z;
                sscanf(line.c_str(), "%c %f %f %f", &c, &x, &y, &z);
                if (c == 'v') {
                    vertices.push_back({x, y, z});
                } else if (c == 'f') {
                    indices.push_back((unsigned int)x);
                    indices.push_back((unsigned int)y);
                    indices.push_back((unsigned int)z);
                }
            }
            file.close();
        } else {
            std::cout << "Failed to open .obj file for reading vertices." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    int Renderer::create_application()
    {
        OpenGP::Application app;
        OpenGP::Shader shader;
        Mesh mesh;

        // Read vertices and faces from .obj file
        std::vector<OpenGP::Vec3> vertices;
        std::vector<unsigned int> indices;
        read_vertices_and_indices_from_file(m_mesh_path, vertices, indices);
        mesh.load_vertices(vertices, indices);

        shader.add_vshader_from_source(
            read_file_to_string(
                "/Users/michael/Dropbox/Programming/icg/assignment2/Shaders/vertex_shader.glsl").c_str());
        shader.add_fshader_from_source(
            read_file_to_string(
                "/Users/michael/Dropbox/Programming/icg/assignment2/Shaders/fragment_shader.glsl").c_str());
        shader.link();

        app.add_listener<OpenGP::ApplicationUpdateEvent>([this](const OpenGP::ApplicationUpdateEvent& aue) {
            update();
        });

        OpenGP::Window& window = app.create_window([this, &shader, &mesh](const OpenGP::Window& window) {
            draw(shader, mesh);
        });
        window.set_title("Assignment 2");
        window.set_size(m_width, m_height);

        return app.run();
    }
}
