#include "Rendering/renderer.h"

int main(int argc, char** argv)
{
    constexpr int width = 720;
    constexpr int height = 720;
    constexpr float speed_factor = 1.0f;

    // Render a 2D animation using OpenGL
    Rendering::Renderer renderer {
        width,
        height,
        speed_factor
    };
    return renderer.create_application();
}
