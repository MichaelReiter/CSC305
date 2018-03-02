#include "renderer.h"

#include <OpenGP/GL/Application.h>
#include <OpenGP/external/LodePNG/lodepng.cpp>
#include <fstream>
#include <vector>
#include <string>

namespace Rendering {
    std::unique_ptr<OpenGP::GPUMesh> quad;
    std::unique_ptr<OpenGP::Shader> quad_shader;
    std::unique_ptr<OpenGP::Shader> framebuffer_shader;
    std::unique_ptr<OpenGP::RGBA8Texture> cat;
    std::unique_ptr<OpenGP::RGBA8Texture> stars;
    std::unique_ptr<OpenGP::Framebuffer> framebuffer;
    std::unique_ptr<OpenGP::RGBA8Texture> color_buffer_texture;

    Renderer::Renderer(unsigned int width, unsigned int height, float speed_factor) :
        m_width(width),
        m_height(height),
        m_speed_factor(speed_factor)
    {}

    Renderer::~Renderer() {}

    std::string Renderer::load_source(const char* filename) {
        std::ifstream f(filename);
        std::stringstream buffer;
        buffer << f.rdbuf();
        return buffer.str();
    }

    void Renderer::load_texture(std::unique_ptr<OpenGP::RGBA8Texture>& texture,
                                const std::string& filename)
    {
        std::vector<unsigned char> image;
        unsigned int width;
        unsigned int height;
        unsigned error = lodepng::decode(image, width, height, filename.c_str());
        if (error) {
            std::cout << "decoder error " << error
                << ": " << lodepng_error_text(error) << std::endl;
        }
        unsigned char* row = new unsigned char[4 * width];
        for (int i = 0; i < int(height) / 2; i++) {
            memcpy(row, &image[4 * i * width], 4 * width * sizeof(unsigned char));
            memcpy(&image[4 * i * width],
                &image[image.size() - 4 * (i + 1) * width],
                4 * width * sizeof(unsigned char));
            memcpy(&image[image.size() - 4 * (i + 1) * width],
                   row,
                   4 * width * sizeof(unsigned char));
        }
        delete row;
        texture = std::unique_ptr<OpenGP::RGBA8Texture>(new OpenGP::RGBA8Texture());
        texture->upload_raw(width, height, &image[0]);
    }

    void Renderer::init()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        std::string fb_vshader = load_source(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/fb_vshader.glsl");
        std::string fb_fshader = load_source(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/fb_fshader.glsl");
        std::string quad_vshader = load_source(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/quad_vshader.glsl");
        std::string quad_fshader = load_source(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/quad_fshader.glsl");

        framebuffer_shader = std::unique_ptr<OpenGP::Shader>(new OpenGP::Shader());
        framebuffer_shader->verbose = true;
        framebuffer_shader->add_vshader_from_source(fb_vshader.c_str());
        framebuffer_shader->add_fshader_from_source(fb_fshader.c_str());
        framebuffer_shader->link();

        quad_shader = std::unique_ptr<OpenGP::Shader>(new OpenGP::Shader());
        quad_shader->verbose = true;
        quad_shader->add_vshader_from_source(quad_vshader.c_str());
        quad_shader->add_fshader_from_source(quad_fshader.c_str());
        quad_shader->link();

        quad = std::unique_ptr<OpenGP::GPUMesh>(new OpenGP::GPUMesh());
        std::vector<OpenGP::Vec3> quad_vposition {
            {-1, -1, 0},
            {-1, 1, 0},
            {1, -1, 0},
            {1, 1, 0}
        };
        quad->set_vbo<OpenGP::Vec3>("vposition", quad_vposition);
        std::vector<unsigned int> quad_triangle_indices {
            0, 2, 1, 1, 2, 3
        };
        quad->set_triangles(quad_triangle_indices);
        std::vector<OpenGP::Vec2> quad_vtexcoord {
            {0, 0},
            {0,  1},
            {1, 0},
            {1,  1}
        };
        quad->set_vtexcoord(quad_vtexcoord);

        load_texture(cat,
                     "/Users/michael/Dropbox/Programming/icg/data/nyancat.png");
        load_texture(stars,
                     "/Users/michael/Dropbox/Programming/icg/data/background.png");
    }

    void Renderer::draw_scene()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        float t = glfwGetTime() * m_speed_factor;
        Transform transform = Transform::Identity();
        // background.draw(transform.matrix());
        quad_shader->bind();
        quad_shader->set_uniform("M", transform.matrix());
        // Make texture unit 0 active
        glActiveTexture(GL_TEXTURE0);
        // Bind the texture to the active unit for drawing
        stars->bind();
        // Set the shader's texture uniform to the index of the texture unit we have
        // bound the texture to
        quad_shader->set_uniform("tex", 0);
        quad->set_attributes(*quad_shader);
        quad->draw();
        stars->unbind();

        float x_coordinate = 0.7f * std::cos(t);
        float y_coordinate = 0.7f * std::sin(t);
        transform *= Eigen::Translation3f(x_coordinate, y_coordinate, 0.0f);
        transform *= Eigen::AngleAxisf(t + M_PI / 2, Eigen::Vector3f::UnitZ());
        transform *= Eigen::AlignedScaling3f(0.2f, 0.2f, 1.0f);

        quad_shader->bind();
        quad_shader->set_uniform("M", transform.matrix());
        // Make texture unit 0 active
        glActiveTexture(GL_TEXTURE0);
        // Bind the texture to the active unit for drawing
        cat->bind();
        // Set the shader's texture uniform to the index
        // of the texture unit we have bound the texture to
        quad_shader->set_uniform("tex", 0);
        quad->set_attributes(*quad_shader);
        quad->draw();
        cat->unbind();
        quad_shader->unbind();

        glDisable(GL_BLEND);
    }

    int Renderer::create_application()
    {
        OpenGP::Application app;
        init();

        color_buffer_texture = std::unique_ptr<OpenGP::RGBA8Texture>(new OpenGP::RGBA8Texture());
        color_buffer_texture->allocate(m_width, m_height);
        framebuffer = std::unique_ptr<OpenGP::Framebuffer>(new OpenGP::Framebuffer());
        framebuffer->attach_color_texture(*color_buffer_texture);

        OpenGP::Window& window = app.create_window([this](OpenGP::Window&) {
            glViewport(0, 0, m_width, m_height);

            framebuffer->bind();
            glClear(GL_COLOR_BUFFER_BIT);
            draw_scene();
            framebuffer->unbind();

            glViewport(0, 0, m_width, m_height);
            glClear(GL_COLOR_BUFFER_BIT);
            framebuffer_shader->bind();

            // Bind texture and set uniforms
            glActiveTexture(GL_TEXTURE0);
            color_buffer_texture->bind();
            framebuffer_shader->set_uniform("tex", 0);
            framebuffer_shader->set_uniform("tex_width", float(m_width));
            framebuffer_shader->set_uniform("tex_height", float(m_height));

            quad->set_attributes(*framebuffer_shader);
            quad->draw();
            color_buffer_texture->unbind();
            framebuffer_shader->unbind();
        });
        window.set_title("Framebuffer Animation");
        window.set_size(m_width, m_height);

        return app.run();
    }
}
