#include "Rendering/renderer.h"

int main(int argc, char** argv)
{
    constexpr unsigned int width = 1280;
    constexpr unsigned int height = 720;
    // The index at which we begin a new OpenGL triangle strip
    constexpr unsigned int restart_primitive = 999999;

    // Procedurally generate and render a 3D environment using OpenGL
    Rendering::Renderer renderer {
        width,
        height,
        restart_primitive
    };
    return renderer.create_application();
}
