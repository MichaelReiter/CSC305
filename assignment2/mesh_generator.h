#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include <fstream>

class MeshGenerator {
public:
    MeshGenerator();

    ~MeshGenerator();

    enum Shape: unsigned int {
        Cube = 0,
        Sphere,
        Cylinder
    };

    void generate_obj_file(enum Shape shape, const std::string& filename) const;

private:
    void write_cube_obj(std::ofstream& file) const;
    void write_sphere_obj(std::ofstream& file) const;
    void write_cylinder_obj(std::ofstream& file) const;
};

#endif // MESH_GENERATOR_H
