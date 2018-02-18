#include "cube_generator.h"
#include "cylinder_generator.h"
#include "renderer.h"
#include "sphere_generator.h"

int main(int argc, char** argv)
{
    constexpr unsigned int width = 800;
    constexpr unsigned int height = 600;
    constexpr unsigned int polygon_resolution = 30;

    // Generate .obj mesh files by mathematically approximating shapes
    CubeGenerator cube_generator = CubeGenerator();
    SphereGenerator sphere_generator = SphereGenerator(polygon_resolution);
    CylinderGenerator cylinder_generator = CylinderGenerator(polygon_resolution);
    cube_generator.generate_obj_file("cube.obj");
    sphere_generator.generate_obj_file("sphere.obj");
    cylinder_generator.generate_obj_file("cylinder.obj");

    Renderer renderer = {width, height};
    return renderer.create_application();
}
