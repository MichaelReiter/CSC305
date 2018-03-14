#include "Rendering/renderer.h"

int main(int argc, char** argv)
{
    constexpr unsigned int width = 1280;
    constexpr unsigned int height = 720;
    // The index at which we begin a new triangle strip
    constexpr unsigned int res_prim = 999999;

    // Procedurally generate and render a 3D environment using OpenGL
    Rendering::Renderer renderer {
        width,
        height,
        res_prim
    };
    return renderer.create_application();
}
