#include <iostream>
#include <sstream>
#include <vector>
#include "mesh_generator.h"

namespace MeshGenerators {
    MeshGenerator::MeshGenerator() {}

    MeshGenerator::~MeshGenerator() {}

    void MeshGenerator::write_vertices(std::ofstream& file, const std::vector<OpenGP::Vec3>& vertices) const
    {
        file << "# vertices" << std::endl;
        for (const auto& v : vertices) {
            file << "v " << std::fixed << v[0] << " " << v[1] << " " << v[2] << std::endl;
        }
    }

    void MeshGenerator::write_indices(std::ofstream& file,
                                      const std::vector<OpenGP::Vec3>& faces,
                                      const std::vector<OpenGP::Vec3>& normals) const
    {
        file << "# indices" << std::endl;
        for (int i = 0; i < faces.size(); i++) {
            file << "f " << (int)faces[i][0] << "//" << (int)normals[i][0]
                  << " " << (int)faces[i][1] << "//" << (int)normals[i][1]
                  << " " << (int)faces[i][2] << "//" << (int)normals[i][2]
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
