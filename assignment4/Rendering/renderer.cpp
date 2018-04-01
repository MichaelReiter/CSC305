#include <cmath>
#include <fstream>
#include "../Noise/noise.h"
#include <OpenGP/GL/Application.h>
#include "OpenGP/GL/Eigen.h"
#include "renderer.h"
#include <string>
#include "../Texture/texture.h"
#include <vector>

namespace Rendering {
    std::unique_ptr<OpenGP::Shader> skybox_shader;
    std::unique_ptr<OpenGP::GPUMesh> skybox_mesh;
    GLuint skybox_texture;

    std::unique_ptr<OpenGP::Shader> terrain_shader;
    std::unique_ptr<OpenGP::GPUMesh> terrain_mesh;
    std::unique_ptr<OpenGP::R32FTexture> height_texture;
    std::map<std::string, std::unique_ptr<OpenGP::RGBA8Texture>> terrain_textures;

    Renderer::Renderer(unsigned int width,
                       unsigned int height,
                       unsigned int restart_primitive) :
        m_width(width),
        m_height(height),
        m_restart_primitive(restart_primitive),
        m_camera_position({0.0f, 0.0f, 3.0f}),
        m_camera_front({0.0f, -1.0f, 0.0f}),
        m_camera_up({0.0f, 0.0f, 1.0f}),
        m_yaw(0.0f),
        m_pitch(0.0f)
    {}

    Renderer::~Renderer() {}

    std::string Renderer::load_shader(const std::string& filename) const
    {
        std::ifstream f(filename.c_str());
        std::stringstream buffer;
        buffer << f.rdbuf();
        return buffer.str();
    }

    void Renderer::init()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        std::string skybox_vshader = load_shader(
            "/Users/michael/Dropbox/Programming/icg/assignment4/Shaders/skybox_vshader.glsl");
        std::string skybox_fshader = load_shader(
            "/Users/michael/Dropbox/Programming/icg/assignment4/Shaders/skybox_fshader.glsl");
        std::string terrain_vshader = load_shader(
            "/Users/michael/Dropbox/Programming/icg/assignment4/Shaders/terrain_vshader.glsl");
        std::string terrain_fshader = load_shader(
            "/Users/michael/Dropbox/Programming/icg/assignment4/Shaders/terrain_fshader.glsl");

        // Compile shaders
        skybox_shader = std::unique_ptr<OpenGP::Shader>(new OpenGP::Shader());
        skybox_shader->verbose = true;
        skybox_shader->add_vshader_from_source(skybox_vshader.c_str());
        skybox_shader->add_fshader_from_source(skybox_fshader.c_str());
        skybox_shader->link();

        terrain_shader = std::unique_ptr<OpenGP::Shader>(new OpenGP::Shader());
        terrain_shader->verbose = true;
        terrain_shader->add_vshader_from_source(terrain_vshader.c_str());
        terrain_shader->add_fshader_from_source(terrain_fshader.c_str());
        terrain_shader->link();

        // Get height texture
        height_texture = std::unique_ptr<OpenGP::R32FTexture>(Noise::fBm2DTexture());

        // Load terrain and cubemap textures
        std::string asset_directory = "/Users/michael/Dropbox/Programming/icg/assignment4/assets/";
        const std::vector<std::string> material_list {
            "grass",
            "rock",
            "sand",
            "snow",
            "water"
        };
        for (const std::string& material : material_list) {
            Texture::load_texture(terrain_textures[material],
                                  (asset_directory + material + ".png").c_str());
            terrain_textures[material]->bind();
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

        // Load sky textures
        const std::vector<std::string> sky_list {
            "miramar_ft",
            "miramar_bk",
            "miramar_dn",
            "miramar_up",
            "miramar_rt",
            "miramar_lf"
        };
        glGenTextures(1, &skybox_texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
        for (int i = 0; i < sky_list.size(); i++) {
            std::vector<unsigned char> image;
            Texture::load_texture(image, (asset_directory + sky_list[i] + ".png").c_str());
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0,
                         GL_RGBA,
                         1024,
                         1024,
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         &image[0]);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void Renderer::generate_terrain_mesh()
    {
        // Create a flat (z=0) mesh for the terrain with given dimensions, using triangle strips
        terrain_mesh = std::unique_ptr<OpenGP::GPUMesh>(new OpenGP::GPUMesh());
        // Grid resolution
        int n_width = 256;
        int n_height = 256;
        // Grid width, centered at (0, 0)
        float f_width = 5.0f;
        float f_height = 5.0f;

        // Vertex positions, texture coords
        std::vector<OpenGP::Vec3> points;
        std::vector<OpenGP::Vec2> texture_coordinates;
        for (int j = 0; j < n_height; j++) {
            for (int i = 0; i < n_width; i++) {
                // TODO: calculate vertex positions, texture indices done for you
                OpenGP::Vec2 coordinate {i / (float)(n_width - 1), j / (float)(n_height - 1)};
                points.push_back({0.0f, 0.0f, 0.0f});
                texture_coordinates.push_back(coordinate);
            }
        }

        // Element indices using triangle strips
        std::vector<unsigned int> indices;
        for (int j = 0; j < n_height - 1; j++) {
            // The two vertices at the base of each strip
            indices.push_back(j * n_width);
            indices.push_back((j + 1) * n_width);
            for (int i = 1; i < n_width; i++) {
                // TODO: push_back next two vertices
                // HINT: Each one will generate a new triangle
                indices.push_back(0);
                indices.push_back(0);
            }
            // A new strip will begin when this index is reached
            indices.push_back(m_restart_primitive);
        }

        terrain_mesh->set_vbo<OpenGP::Vec3>("vposition", points);
        terrain_mesh->set_triangles(indices);
        terrain_mesh->set_vtexcoord(texture_coordinates);
    }

    void Renderer::generate_cube_mesh()
    {
        // Generate a cube mesh for skybox, already done
        skybox_mesh = std::unique_ptr<OpenGP::GPUMesh>(new OpenGP::GPUMesh());
        std::vector<OpenGP::Vec3> points;
        points.push_back({ 1, 1, 1}); // 0
        points.push_back({-1, 1, 1}); // 1
        points.push_back({ 1, 1,-1}); // 2
        points.push_back({-1, 1,-1}); // 3
        points.push_back({ 1,-1, 1}); // 4
        points.push_back({-1,-1, 1}); // 5
        points.push_back({-1,-1,-1}); // 6
        points.push_back({ 1,-1,-1}); // 7
        std::vector<unsigned int> indices { 3, 2, 6, 7, 4, 2, 0, 3, 1, 6, 5, 4, 1, 0 };
        skybox_mesh->set_vbo<OpenGP::Vec3>("vposition", points);
        skybox_mesh->set_triangles(indices);
    }

    void Renderer::draw_skybox()
    {
        skybox_shader->bind();

        // Set transformations
        OpenGP::Vec3 look = m_camera_front + m_camera_position;
        OpenGP::Mat4x4 view = OpenGP::lookAt(m_camera_position, look, m_camera_up);
        skybox_shader->set_uniform("V", view);
        OpenGP::Mat4x4 projection = OpenGP::perspective(80.0f,
                                                        m_width / (float)m_height,
                                                        0.1f,
                                                        60.0f);
        skybox_shader->set_uniform("P", projection);

        // TODO: Bind Textures and set uniform
        // HINT: Use GL_TEXTURE0, and texture type GL_TEXTURE_CUBE_MAP

        // TODO: Set atrributes, draw cube using GL_TRIANGLE_STRIP mode
        glEnable(GL_DEPTH_TEST);

        skybox_shader->unbind();
    }

    void Renderer::draw_terrain()
    {
        terrain_shader->bind();

        // TODO: Create transformation matrices HINT: use lookAt and perspective
        OpenGP::Mat4x4 model = OpenGP::Mat4x4::Identity(); // Identity should be fine
        terrain_shader->set_uniform("M", model);

        OpenGP::Vec3 look = m_camera_front + m_camera_position;
        OpenGP::Mat4x4 view = OpenGP::Mat4x4::Identity(); // here
        terrain_shader->set_uniform("V", view);

        OpenGP::Mat4x4 projection = OpenGP::Mat4x4::Identity(); // and here
        terrain_shader->set_uniform("P", projection);

        terrain_shader->set_uniform("viewPos", m_camera_position);

        // Bind textures
        int i = 0;
        std::map<std::string, std::unique_ptr<OpenGP::RGBA8Texture>>::iterator it
            = terrain_textures.begin();
        for (; it != terrain_textures.end(); it++) {
            glActiveTexture(GL_TEXTURE1 + i);
            (it->second)->bind();
            terrain_shader->set_uniform(it->first.c_str(), i + 1);
            i++;
        }
        // TODO: Bind height texture to GL_TEXTURE0 and set uniform noiseTex

        // Draw terrain using triangle strips
        glEnable(GL_DEPTH_TEST);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        terrain_mesh->set_attributes(*terrain_shader);
        terrain_mesh->set_mode(GL_TRIANGLE_STRIP);
        glEnable(GL_PRIMITIVE_RESTART);
        glPrimitiveRestartIndex(m_restart_primitive);
        // TODO: Uncomment line below once this function is implemented
        // terrain_mesh->draw();

        terrain_shader->unbind();
    }

    int Renderer::create_application()
    {
        OpenGP::Application app;
        init();

        generate_cube_mesh();
        generate_terrain_mesh();

        // Display callback
        OpenGP::Window& window = app.create_window([&](OpenGP::Window& window) {
            glViewport(0, 0, m_width, m_height);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            draw_skybox();
            glClear(GL_DEPTH_BUFFER_BIT);
            draw_terrain();
        });
        window.set_title("Assignment 4");
        window.set_size(m_width, m_height);

        OpenGP::Vec2 mouse(0, 0);
        window.add_listener<OpenGP::MouseMoveEvent>([&](const OpenGP::MouseMoveEvent &m) {
            // Camera control
            OpenGP::Vec2 delta = m.position - mouse;
            delta[1] = -delta[1];
            float sensitivity = 0.005f;
            delta = sensitivity * delta;

            m_yaw += delta[0];
            m_pitch += delta[1];

            if (m_pitch > M_PI / 2.0f - 0.01f) {
                m_pitch =  M_PI / 2.0f - 0.01f;
            }
            if (m_pitch < -M_PI / 2.0f + 0.01f) {
                m_pitch = -M_PI / 2.0f + 0.01f;
            }
            OpenGP::Vec3 front(0.0f, 0.0f, 0.0f);
            front[0] = sin(m_yaw) * cos(m_pitch);
            front[1] = cos(m_yaw) * cos(m_pitch);
            front[2] = sin(m_pitch);

            m_camera_front = front.normalized();
            mouse = m.position;
        });

        window.add_listener<OpenGP::KeyEvent>([&](const OpenGP::KeyEvent& k) {
            // TODO: Implement WASD keys HINT: compare k.key to GLFW_KEY_W
        });

        return app.run();
    }
}
