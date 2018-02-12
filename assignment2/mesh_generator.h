#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include <fstream>
#include <string>

class MeshGenerator {
public:
    MeshGenerator();

    ~MeshGenerator();    

    enum Shape: unsigned int {
        Cube = 0,
        Sphere,
        Cylinder
    };

    void generate_obj_file(enum Shape shape, std::string filename);

private:
    void write_cube_obj(std::ofstream& file);
    void write_sphere_obj(std::ofstream& file);
    void write_cylinder_obj(std::ofstream& file);
};

#endif // MESH_GENERATOR_H
