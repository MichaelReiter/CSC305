#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>
#include "mesh_generator.h"

MeshGenerator::MeshGenerator() {}

MeshGenerator::~MeshGenerator() {}

void MeshGenerator::write_vertices(std::ofstream& file, const std::vector<OpenGP::Vec3>& vertices) const
{
    file << "# vertices" << std::endl;
    for (const auto& v : vertices) {
        file << "v " << std::fixed << v[0] << " " << v[1] << " " << v[2] << std::endl;
    }
}

void MeshGenerator::write_faces(std::ofstream& file, const std::vector<OpenGP::Vec3>& faces) const
{
    file << "# faces" << std::endl;
    for (const auto& f : faces) {
        file << "f " << (int)f[0] << " " << (int)f[1] << " " << (int)f[2] << std::endl;
    }
}

void MeshGenerator::write_cube_obj(std::ofstream& file) const
{
    file << "# cube" << std::endl;
    std::vector<OpenGP::Vec3> vertices {
        {-1, 1, -1},
        {-1, 1, 1},
        {1, 1, 1},
        {1, 1, -1},
        {-1, -1, -1},
        {-1, -1, 1},
        {1, -1, 1},
        {1, -1, -1}
    };
    write_vertices(file, vertices);
    std::vector<OpenGP::Vec3> faces {
        {6, 2, 1},
        {6, 1, 5},
        {8, 4, 3},
        {8, 3, 7},
        {8, 7, 6},
        {8, 6, 5},
        {3, 4, 1},
        {3, 1, 2},
        {5, 1, 4},
        {5, 4, 8},
        {7, 3, 2},
        {7, 2, 6}
    };
    write_faces(file, faces);    
}

void MeshGenerator::write_sphere_obj(std::ofstream& file) const
{
    file << "# sphere" << std::endl;
    
}

void MeshGenerator::write_cylinder_obj(std::ofstream& file) const
{
    constexpr int resolution = 30;
    file << "# cylinder" << std::endl;
    std::vector<OpenGP::Vec3> vertices;
    // Points around circles
    for (int i = 0; i < resolution; i++) {
        float theta = (2 * M_PI * i) / (resolution - 1);
        vertices.push_back({sin(theta), 1.0f, cos(theta)});
        vertices.push_back({sin(theta), -1.0f, cos(theta)});
    }
    // North and South poles in the center of the circles
    vertices.push_back({0, 1.0f, 0});
    vertices.push_back({0, -1.0f, 0});
    write_vertices(file, vertices);
    std::vector<OpenGP::Vec3> faces;
    // Faces on the rectangle
    for (int i = 1; i < resolution*2; i++) {
        faces.push_back({(i), (i+1), (i+2)});
        faces.push_back({(i+1), (i+3), (i+2)});
    }
    // Faces on the circles
    for (int i = 1; i < 2 * resolution - 1; i++) {
        if (i % 2 == 0) {
            faces.push_back({i, 2*resolution+2, i+2});
        } else {
            faces.push_back({i+2, 2*resolution+1, i});
        }
    }
    write_faces(file, faces);
}

void MeshGenerator::generate_obj_file(enum Shape shape, const std::string& filename) const
{
    std::ofstream file(filename);
    if (file.is_open()) {
        switch (shape) {
        case Cube:
            write_cube_obj(file);
            break;
        case Sphere:
            write_sphere_obj(file);
            break;
        case Cylinder:
            write_cylinder_obj(file);
            break;
        default:
            std::cout << "Error: invalid shape" << std::endl;
            exit(EXIT_FAILURE);
        }
        file.close();
    } else {
        std::cout << "Failed to open .obj file for mesh creation." << std::endl;
        exit(EXIT_FAILURE);
    }
}
