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
                       float field_of_view,
                       float movement_speed) :
        m_width(width),
        m_height(height),
        m_restart_primitive(999999),
        m_movement_speed(movement_speed),
        m_field_of_view(field_of_view),
        m_camera_position({0.0f, 0.0f, 10.0f}),
        m_camera_front({0.0f, -1.0f, 0.0f}),
        m_camera_up({0.0f, 0.0f, 1.0f}),
        m_yaw(0.0f),
        m_pitch(0.0f),
        m_wave_motion_factor(0.0f)
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
            "water",
            "water2"
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

    void Renderer::generate_terrain_mesh(int resolution, float side_length)
    {
        // Create a flat mesh for the terrain with given dimensions, using triangle strips
        terrain_mesh = std::unique_ptr<OpenGP::GPUMesh>(new OpenGP::GPUMesh());

        // Vertex positions and texture coords
        std::vector<OpenGP::Vec3> points;
        std::vector<OpenGP::Vec2> texture_coordinates;
        for (int j = 0; j < resolution; j++) {
            for (int i = 0; i < resolution; i++) {
                points.push_back({-side_length / 2 + j / (float)resolution * side_length,
                                  -side_length / 2 + i / (float)resolution * side_length,
                                  0.0f});
                texture_coordinates.push_back({i / (float)(resolution - 1),
                                               j / (float)(resolution - 1)});
            }
        }

        // Element indices using triangle strips
        std::vector<unsigned int> indices;
        for (int j = 0; j < resolution - 1; j++) {
            // The two vertices at the base of each strip
            indices.push_back(j * resolution);
            indices.push_back((j + 1) * resolution);
            for (int i = 1; i < resolution; i++) {
                indices.push_back(i + j * resolution);
                indices.push_back(i + (j + 1) * resolution);
            }
            // A new strip begins when this index is reached
            indices.push_back(m_restart_primitive);
        }

        terrain_mesh->set_vbo<OpenGP::Vec3>("vposition", points);
        terrain_mesh->set_triangles(indices);
        terrain_mesh->set_vtexcoord(texture_coordinates);
    }

    void Renderer::generate_cube_mesh()
    {
        // Generate a cube mesh for skybox
        skybox_mesh = std::unique_ptr<OpenGP::GPUMesh>(new OpenGP::GPUMesh());
        std::vector<OpenGP::Vec3> points;
        points.push_back({ 2, 2, 2}); // 0
        points.push_back({-2, 2, 2}); // 1
        points.push_back({ 2, 2,-2}); // 2
        points.push_back({-2, 2,-2}); // 3
        points.push_back({ 2,-2, 2}); // 4
        points.push_back({-2,-2, 2}); // 5
        points.push_back({-2,-2,-2}); // 6
        points.push_back({ 2,-2,-2}); // 7
        std::vector<unsigned int> indices {3, 2, 6, 7, 4, 2, 0, 3, 1, 6, 5, 4, 1, 0};
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
        OpenGP::Mat4x4 projection = OpenGP::perspective(m_field_of_view,
                                                        m_width / (float)m_height,
                                                        0.1f,
                                                        100.0f);
        skybox_shader->set_uniform("P", projection);

        // Bind skybox texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
        skybox_shader->set_uniform("noiseTex", 0);

        // Set atrributes and draw cube using GL_TRIANGLE_STRIP mode
        glEnable(GL_DEPTH_TEST);
        skybox_mesh->set_attributes(*skybox_shader);
        skybox_mesh->set_mode(GL_TRIANGLE_STRIP);
        glEnable(GL_PRIMITIVE_RESTART);
        glPrimitiveRestartIndex(m_restart_primitive);
        skybox_mesh->draw();

        skybox_shader->unbind();
    }

    void Renderer::draw_terrain()
    {
        terrain_shader->bind();

        // Create transformation matrices
        OpenGP::Mat4x4 model = OpenGP::Mat4x4::Identity();
        terrain_shader->set_uniform("M", model);

        OpenGP::Vec3 look = m_camera_front + m_camera_position;
        OpenGP::Mat4x4 view = OpenGP::lookAt(m_camera_position, look, m_camera_up);
        terrain_shader->set_uniform("V", view);

        OpenGP::Mat4x4 projection = OpenGP::perspective(m_field_of_view,
                                                        m_width / (float)m_height,
                                                        0.1f,
                                                        100.0f);
        terrain_shader->set_uniform("P", projection);

        terrain_shader->set_uniform("viewPos", m_camera_position);

        // Bind textures
        int i = 0;
        for (const auto& texture : terrain_textures) {
            glActiveTexture(GL_TEXTURE1 + i);
            (texture.second)->bind();
            terrain_shader->set_uniform(texture.first.c_str(), i + 1);
            i++;
        }
        // Bind height texture to GL_TEXTURE0 and set uniform noiseTex
        glActiveTexture(GL_TEXTURE0);
        height_texture->bind();
        terrain_shader->set_uniform("noiseTex", 0);

        // Draw terrain using triangle strips
        glEnable(GL_DEPTH_TEST);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        terrain_mesh->set_attributes(*terrain_shader);
        terrain_mesh->set_mode(GL_TRIANGLE_STRIP);
        glEnable(GL_PRIMITIVE_RESTART);
        glPrimitiveRestartIndex(m_restart_primitive);
        terrain_mesh->draw();

        terrain_shader->set_uniform("wave_motion", m_wave_motion_factor);
        m_wave_motion_factor += 0.0001f;
        if (m_wave_motion_factor > 1.0f) {
            m_wave_motion_factor = 0.0f;
        }

        height_texture->unbind();
        terrain_shader->unbind();
    }

    int Renderer::create_application()
    {
        OpenGP::Application app;
        init();

        generate_cube_mesh();
        generate_terrain_mesh(64, 40.0f);

        // Display callback
        OpenGP::Window& window = app.create_window([&](OpenGP::Window& window) {
            glViewport(0, 0, m_width * 2, m_height * 2);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            draw_skybox();
            glClear(GL_DEPTH_BUFFER_BIT);
            draw_terrain();
        });
        window.set_title("Procedurally Generated Mountainous Terrain");
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

        // WASD camera controls
        window.add_listener<OpenGP::KeyEvent>([&](const OpenGP::KeyEvent& k) {
            if (k.key == GLFW_KEY_W) {
                m_camera_position = m_camera_position + m_movement_speed * m_camera_front.normalized();
            }
            if (k.key == GLFW_KEY_A) {
                m_camera_position = m_camera_position - m_movement_speed * m_camera_front.normalized().cross(m_camera_up);
            }
            if (k.key == GLFW_KEY_S) {
                m_camera_position = m_camera_position - m_movement_speed * m_camera_front.normalized();
            }
            if (k.key == GLFW_KEY_D) {
                m_camera_position = m_camera_position + m_movement_speed * m_camera_front.normalized().cross(m_camera_up);
            }
        });

        return app.run();
    }
}
