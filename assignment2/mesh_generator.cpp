#include <iostream>
#include "mesh_generator.h"

MeshGenerator::MeshGenerator() {}

MeshGenerator::~MeshGenerator() {}

void MeshGenerator::write_cube_obj(std::ofstream& file)
{
    file << "# cube" << std::endl;
}

void MeshGenerator::write_sphere_obj(std::ofstream& file)
{
    file << "# sphere" << std::endl;
}

void MeshGenerator::write_cylinder_obj(std::ofstream& file)
{
    file << "# cylinder" << std::endl;
}

void MeshGenerator::generate_obj_file(enum Shape shape, std::string filename)
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
            std::cout << "Error" << std::endl;
            exit(EXIT_FAILURE);
        }
        file.close();
    } else {
        std::cout << "Failed to open .obj file for mesh creation." << std::endl;
        exit(EXIT_FAILURE);
    }
}
