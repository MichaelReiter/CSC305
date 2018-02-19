#include <fstream>
#include <OpenGP/GL/Application.h>
#include <OpenGP/GL/Eigen.h>
#include <math.h>
#include "renderer.h"

Renderer::Renderer(unsigned int width, unsigned int height) :
    m_width(width),
    m_height(height)
{
    m_time = 0.0f;
}

Renderer::~Renderer() {}

void Renderer::draw()
{
    glViewport(0, 0, m_width, m_height);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader->bind();

    OpenGP::Mat4x4 model = OpenGP::Mat4x4::Identity();

    // Now that we are drawing a 3D scene, we need to define the paramters of
    // the camera, starting with its position
    OpenGP::Vec3 camera_position = {5 * cosf(m_time), 2, 5 * sinf(m_time)};

    // This helper function generates a view matrix for us
    OpenGP::Mat4x4 view = OpenGP::look_at(camera_position, OpenGP::Vec3(0, 1, 0), OpenGP::Vec3(0, 1, 0));

    // To build the projection matrix we need the window's aspect ratio
    float ratio = float(m_width) / float(m_height);

    // Another helper function to build the projection matrix
    OpenGP::Mat4x4 projection = OpenGP::perspective(60, ratio, 0.1, 50);

    // The combined model, view, and projection transformation matrix
    OpenGP::Mat4x4 MVP = projection * view * model;

    // We need to apply any rotation from the model matrix to the vertex normals
    // while ignoring any translation or scaling
    OpenGP::Mat4x4 model_rotation = model.inverse().transpose();

    // Pass some camera related values that we need in the shaders
    m_shader->set_uniform("model", model);
    m_shader->set_uniform("model_rotation", model_rotation);
    m_shader->set_uniform("MVP", MVP);
    m_shader->set_uniform("camera_position", camera_position);

    // Set the material and lighting paramters
    OpenGP::Vec3 light_direction = OpenGP::Vec3(cosf(2 * m_time), -1, sinf(2 * m_time)).normalized();
    m_shader->set_uniform("light_direction", light_direction);
    m_shader->set_uniform("light_color", OpenGP::Vec3(1.0f, 1.0f, 1.0f));
    m_shader->set_uniform("ambient_coefficient", OpenGP::Vec3(0.25f, 0.25f, 0.25f));
    m_shader->set_uniform("object_ambient", OpenGP::Vec3(1.0f, 0.0f, 0.0f));
    m_shader->set_uniform("object_diffuse", OpenGP::Vec3(1.0f, 0.0f, 0.0f));
    m_shader->set_uniform("object_specular", OpenGP::Vec3(0.6f, 0.6f, 0.6f));
    m_shader->set_uniform("phong_exponent", 32.0f);

    m_mesh->set_attributes(*m_shader);

    m_mesh->draw();

    m_shader->unbind();
}

void Renderer::update()
{
    m_time += 0.005f;
}

std::string Renderer::load_source(const std::string& filename) const
{
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int Renderer::create_application()
{
    OpenGP::Application app;
    m_shader = std::unique_ptr<OpenGP::Shader>(new OpenGP::Shader());
    m_mesh = std::unique_ptr<OpenGP::GPUMesh>(new OpenGP::GPUMesh());

    OpenGP::SurfaceMesh mesh_data;
    mesh_data.read("/Users/michael/Dropbox/Programming/icg/bunny.obj");
    mesh_data.update_vertex_normals();

    m_mesh->init_from_mesh(mesh_data);

    m_shader->add_vshader_from_source(load_source("/Users/michael/Dropbox/Programming/icg/assignment2/shaders/vertex_shader.glsl").c_str());
    m_shader->add_fshader_from_source(load_source("/Users/michael/Dropbox/Programming/icg/assignment2/shaders/fragment_shader.glsl").c_str());
    m_shader->link();

    app.add_listener<OpenGP::ApplicationUpdateEvent>([this](const OpenGP::ApplicationUpdateEvent& aue) {
        update();
    });

    OpenGP::Window& window = app.create_window([this](const OpenGP::Window& window) {
        draw();
    });
    window.set_title("Assignment 2");
    window.set_size(m_width, m_height);

    return app.run();
}
