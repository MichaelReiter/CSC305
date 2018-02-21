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

    void Renderer::read_obj_file(const std::string& filename,
                                 std::vector<OpenGP::Vec3>& vertices,
                                 std::vector<unsigned int>& indices,
                                 std::vector<OpenGP::Vec3>& normals) const
    {
        vertices.push_back({0.0f, 0.0f, 0.0f});
        std::vector<unsigned int> vertex_indices;
        std::vector<unsigned int> texture_coordinate_indices;
        std::vector<unsigned int> normal_indices;
        std::vector<OpenGP::Vec2> texture_coordinates;

        FILE *file = fopen(filename.c_str(), "r");
        if (file == NULL) {
            printf("Could not open file\n");
            return;
        }

        while (true) {
            char lineHeader[128];
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF) {
                break;
            }
            if (strcmp(lineHeader, "v") == 0) {
                // Read vertices
                float x, y, z;
                fscanf(file, "%f %f %f\n", &x, &y, &z);
                vertices.push_back(OpenGP::Vec3(x, y, z));
            } else if (strcmp(lineHeader, "vt") == 0) {
                // Read texture coordinates
                float u, v;
                fscanf(file, "%f %f\n", &u, &v);
                texture_coordinates.push_back(OpenGP::Vec2(u, v));
            } else if (strcmp(lineHeader, "vn") == 0) {
                // Read normal vectors
                float x, y, z;
                fscanf(file, "%f %f %f\n", &x, &y, &z);
                normals.push_back(OpenGP::Vec3(x, y, z));
            } else if (strcmp(lineHeader, "f") == 0) {
                // Read faces
                std::string vertex1;
                std::string vertex2;
                std::string vertex3;
                unsigned int vertex_index[3];
                unsigned int texture_coordinate_index[3];
                unsigned int normal_index[3];
                int matches = fscanf(file,
                                     "%d//%d %d//%d %d//%d\n",
                                     &vertex_index[0],
                                     &normal_index[0],
                                     &vertex_index[1],
                                     &normal_index[1],
                                     &vertex_index[2],
                                     &normal_index[2]);
                indices.push_back(vertex_index[0]);
                indices.push_back(vertex_index[1]);
                indices.push_back(vertex_index[2]);
                texture_coordinate_indices.push_back(texture_coordinate_index[0]);
                texture_coordinate_indices.push_back(texture_coordinate_index[1]);
                texture_coordinate_indices.push_back(texture_coordinate_index[2]);
                normal_indices.push_back(normal_index[0]);
                normal_indices.push_back(normal_index[1]);
                normal_indices.push_back(normal_index[2]);
            }
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
            mesh.draw(m_width, m_height, m_time);
        });
        window.set_title("Assignment 2");
        window.set_size(m_width, m_height);

        return app.run();
    }
}
