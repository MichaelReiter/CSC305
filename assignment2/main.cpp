#include <OpenGP/GL/Application.h>

using namespace OpenGP;

void draw(unsigned int width, unsigned int height)
{
    glViewport(0, 0, width, height);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char** argv)
{
    constexpr unsigned int width = 800;
    constexpr unsigned int height = 600;
    
    // Create main window and set callback function
    Application app;
    Window& window = app.create_window([](Window& window) {
        draw(width, height);
    });
    window.set_title("Assignment 2");
    window.set_size(width, height);

    return app.run();
}
