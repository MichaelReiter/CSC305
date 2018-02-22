#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include <fstream>
#include "OpenGP/Image/Image.h"

namespace MeshGenerators {
    class MeshGenerator {
    public:
        MeshGenerator();

        ~MeshGenerator();

        void generate_obj_file(const std::string& filename) const;

    protected:
        void write_vertices(std::ofstream& file, const std::vector<OpenGP::Vec3>& vertices) const;
        void write_vertex_normals(std::ofstream& file, const std::vector<OpenGP::Vec3>& normals) const;
        void write_faces(std::ofstream& file,
                         const std::vector<OpenGP::Vec3>& vertex_indices,
                         const std::vector<OpenGP::Vec3>& normal_indices) const;
        virtual void write_to_file(std::ofstream& file) const = 0;
    };
}

#endif // MESH_GENERATOR_H
