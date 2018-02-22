#include <iostream>
#include <sstream>
#include <vector>
#include "mesh_generator.h"

namespace MeshGenerators {
    MeshGenerator::MeshGenerator() {}

    MeshGenerator::~MeshGenerator() {}

    void MeshGenerator::write_vertices(std::ofstream& file, const std::vector<OpenGP::Vec3>& vertices) const
    {
        for (const auto& v : vertices) {
            file << "v " << std::fixed << v[0] << " " << v[1] << " " << v[2] << std::endl;
        }
    }

    void MeshGenerator::write_vertex_normals(std::ofstream& file, const std::vector<OpenGP::Vec3>& normals) const
    {
        for (const auto& n : normals) {
            file << "vn " << std::fixed << n[0] << " " << n[1] << " " << n[2] << std::endl;
        }
    }

    void MeshGenerator::write_faces(std::ofstream& file,
                                    const std::vector<OpenGP::Vec3>& vertex_indices,
                                    const std::vector<OpenGP::Vec3>& normal_indices) const
    {
        for (int i = 0; i < vertex_indices.size(); i++) {
            file << "f " << (int)vertex_indices[i][0] << "//" << (int)normal_indices[i][0]
                  << " " << (int)vertex_indices[i][1] << "//" << (int)normal_indices[i][1]
                  << " " << (int)vertex_indices[i][2] << "//" << (int)normal_indices[i][2]
                  << std::endl;
        }
    }

    void MeshGenerator::generate_obj_file(const std::string& filename) const
    {
        std::ofstream file(filename);
        if (file.is_open()) {
            write_to_file(file);
            file.close();
        } else {
            std::cout << "Failed to open .obj file for mesh creation." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}
