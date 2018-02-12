#include "mesh_generator.h"
#include "renderer.h"

int main(int argc, char** argv)
{
    constexpr unsigned int width = 800;
    constexpr unsigned int height = 600;

    MeshGenerator mesh_generator;
    mesh_generator.generate_obj_file(MeshGenerator::Cube, "cube.obj");
    mesh_generator.generate_obj_file(MeshGenerator::Sphere, "sphere.obj");
    mesh_generator.generate_obj_file(MeshGenerator::Cylinder, "cylinder.obj");

    Renderer renderer = {width, height};
    return renderer.create_application();
}
