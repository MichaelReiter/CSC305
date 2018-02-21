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

    void Renderer::draw(Mesh& mesh) const
    {
        mesh.draw(m_width, m_height, m_time);
    }

    void Renderer::read_obj_file(const std::string& filename,
                                 std::vector<OpenGP::Vec3>& vertices,
                                 std::vector<unsigned int>& indices,
                                 std::vector<OpenGP::Vec3>& normals) const
    {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                float x;
                float y;
                float z;
                float xn;
                float yn;
                float zn;
                // TODO: use regex to parse file
                if (line.c_str()[0] == 'v') {
                    sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
                    vertices.push_back({x, y, z});
                    printf("v %f %f %f\n", x, y, z);
                } else if (line.c_str()[0] == 'f') {
                    sscanf(line.c_str(), "f %f//%f %f//%f %f//%f", &x, &xn, &y, &yn, &z, &zn);
                    indices.push_back((unsigned int)x);
                    indices.push_back((unsigned int)y);
                    indices.push_back((unsigned int)z);
                    normals.push_back({xn, yn, zn});
                    printf("f %d//%d %d//%d %d//%d\n", (int)x, (int)xn, (int)y, (int)yn, (int)z, (int)zn);
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
        Mesh mesh;

        // Read vertices and faces from .obj file
        std::vector<OpenGP::Vec3> vertices;
        std::vector<unsigned int> indices;
        std::vector<OpenGP::Vec3> normals;
        read_obj_file(m_mesh_path, vertices, indices, normals);
        mesh.load_vertices(vertices, indices);
        mesh.load_normals(normals);

        app.add_listener<OpenGP::ApplicationUpdateEvent>([this](const OpenGP::ApplicationUpdateEvent& aue) {
            update();
        });

        OpenGP::Window& window = app.create_window([this, &mesh](const OpenGP::Window& window) {
            draw(mesh);
        });
        window.set_title("Assignment 2");
        window.set_size(m_width, m_height);

        return app.run();
    }
}
