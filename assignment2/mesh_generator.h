#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include <fstream>
#include "OpenGP/Image/Image.h"

class MeshGenerator {
public:
    MeshGenerator();

    ~MeshGenerator();

    enum Shape : unsigned int {
        Cube = 0,
        Sphere,
        Cylinder
    };

    void generate_obj_file(enum Shape shape, const std::string& filename) const;

private:
    void write_vertices(std::ofstream& file, const std::vector<OpenGP::Vec3>& vertices) const;
    void write_faces(std::ofstream& file, const std::vector<OpenGP::Vec3>& faces) const;
    void write_cube_obj(std::ofstream& file) const;
    void write_sphere_obj(std::ofstream& file) const;
    void write_cylinder_obj(std::ofstream& file) const;
};

#endif // MESH_GENERATOR_H
