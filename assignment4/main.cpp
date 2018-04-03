#include "Rendering/renderer.h"

int main(int argc, char** argv)
{
    constexpr unsigned int width = 1920;
    constexpr unsigned int height = 1080;
    constexpr float field_of_view = 80.0f;
    constexpr float movement_speed = 0.25f;

    // Procedurally generate and render a 3D environment using OpenGL
    Rendering::Renderer renderer {
        width,
        height,
        field_of_view,
        movement_speed
    };
    return renderer.create_application();
}
