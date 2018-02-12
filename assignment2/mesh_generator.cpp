#include <iostream>
#include <vector>
#include "mesh_generator.h"
#include "OpenGP/Image/Image.h"

MeshGenerator::MeshGenerator() {}

MeshGenerator::~MeshGenerator() {}

void MeshGenerator::write_cube_obj(std::ofstream& file) const
{
    file << "# cube" << std::endl;
    file << "# vertices" << std::endl;
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
    for (const auto& v : vertices) {
        file << "v " << v[0] << " " << v[1] << " " << v[2] << std::endl;
    }
    file << "# faces" << std::endl;
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
    for (const auto& f : faces) {
        file << "f " << f[0] << " " << f[1] << " " << f[2] << std::endl;
    }
}

void MeshGenerator::write_sphere_obj(std::ofstream& file) const
{
    file << "# sphere" << std::endl;
    
}

void MeshGenerator::write_cylinder_obj(std::ofstream& file) const
{
    file << "# cylinder" << std::endl;
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
