#include "Rendering/renderer.h"

int main(int argc, char** argv)
{
    constexpr unsigned int width = 1280;
    constexpr unsigned int height = 720;
    constexpr float field_of_view = 80.0f;
    constexpr float movement_speed = 0.1f;

    // Procedurally generate and render a 3D environment using OpenGL
    Rendering::Renderer renderer {
        width,
        height,
        field_of_view,
        movement_speed
    };
    return renderer.create_application();
}
