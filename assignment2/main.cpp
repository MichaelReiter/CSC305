#include "MeshGenerators/cone_generator.h"
#include "MeshGenerators/cube_generator.h"
#include "MeshGenerators/cylinder_generator.h"
#include "MeshGenerators/pyramid_generator.h"
#include "Renderer/renderer.h"
#include "MeshGenerators/sphere_generator.h"

int main(int argc, char** argv)
{
    constexpr unsigned int width = 800;
    constexpr unsigned int height = 600;
    constexpr unsigned int cone_resolution = 30;    
    constexpr unsigned int cylinder_resolution = 30;
    constexpr unsigned int sphere_resolution = 30;

    // Generate .obj mesh files by mathematically approximating shapes
    ConeGenerator cone_generator {cone_resolution};
    CubeGenerator cube_generator;
    CylinderGenerator cylinder_generator {cylinder_resolution};
    PyramidGenerator pyramid_generator;
    SphereGenerator sphere_generator {sphere_resolution};
    cone_generator.generate_obj_file("cone.obj");
    cube_generator.generate_obj_file("cube.obj");
    cylinder_generator.generate_obj_file("cylinder.obj");
    pyramid_generator.generate_obj_file("pyramid.obj");
    sphere_generator.generate_obj_file("sphere.obj");

    Renderer renderer = {width, height};
    return renderer.create_application();
}
