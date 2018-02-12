#include <OpenGP/GL/Application.h>
#include "renderer.h"

Renderer::Renderer(unsigned int width, unsigned int height) :
    m_width(width),
    m_height(height)
{}

Renderer::~Renderer() {}

void Renderer::draw() const
{
    glViewport(0, 0, m_width, m_height);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

int Renderer::create_application() const
{
    // Create main window and set callback function
    OpenGP::Application app;
    OpenGP::Window& window = app.create_window([this](OpenGP::Window& window) {
        draw();
    });
    window.set_title("Assignment 2");
    window.set_size(m_width, m_height);

    return app.run();
}
