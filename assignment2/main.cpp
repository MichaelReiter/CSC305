#include "MeshGenerators/cone_generator.h"
#include "MeshGenerators/cube_generator.h"
#include "MeshGenerators/cylinder_generator.h"
#include "MeshGenerators/pyramid_generator.h"
#include "MeshGenerators/sphere_generator.h"
#include "Renderer/renderer.h"

int main(int argc, char** argv)
{
    constexpr unsigned int width = 800;
    constexpr unsigned int height = 600;
    constexpr unsigned int cone_resolution = 30;    
    constexpr unsigned int cylinder_resolution = 30;
    constexpr unsigned int sphere_resolution = 30;

    // Generate .obj mesh files by mathematically approximating shapes
    MeshGenerators::ConeGenerator cone_generator {cone_resolution};
    MeshGenerators::CubeGenerator cube_generator;
    MeshGenerators::CylinderGenerator cylinder_generator {cylinder_resolution};
    MeshGenerators::PyramidGenerator pyramid_generator;
    MeshGenerators::SphereGenerator sphere_generator {sphere_resolution};

    cone_generator.generate_obj_file("cone.obj");
    cube_generator.generate_obj_file("cube.obj");
    cylinder_generator.generate_obj_file("cylinder.obj");
    pyramid_generator.generate_obj_file("pyramid.obj");
    sphere_generator.generate_obj_file("sphere.obj");

    // Render a mesh using OpenGL
    Renderer renderer = {width, height};
    return renderer.create_application();
}
