#include "renderer.h"

#include <OpenGP/GL/Application.h>
#include <OpenGP/external/LodePNG/lodepng.cpp>
#include <fstream>
#include <vector>
#include <string>

namespace Rendering {
    typedef Eigen::Transform<float, 3, Eigen::Affine> Transform;

    // Textures
    std::unique_ptr<OpenGP::GPUMesh> quad;
    std::unique_ptr<OpenGP::Shader> quad_shader;
    std::unique_ptr<OpenGP::Shader> framebuffer_shader;
    std::unique_ptr<OpenGP::RGBA8Texture> cat;
    std::unique_ptr<OpenGP::RGBA8Texture> stars;

    // Selection
    std::unique_ptr<OpenGP::GPUMesh> line;
    std::unique_ptr<OpenGP::Shader> line_shader;
    std::vector<OpenGP::Vec2> control_points;
    std::unique_ptr<OpenGP::Shader> selection_shader;
    std::unique_ptr<OpenGP::RGBA8Texture> selection_color;
    std::unique_ptr<OpenGP::D16Texture> selection_depth;

    Renderer::Renderer(unsigned int width,
                       unsigned int height,
                       float speed_factor,
                       float point_size) :
        m_width(width),
        m_height(height),
        m_speed_factor(speed_factor),
        m_point_size(point_size)
    {}

    Renderer::~Renderer() {}

    std::string Renderer::load_source(const std::string& filename) const
    {
        std::ifstream f(filename.c_str());
        std::stringstream buffer;
        buffer << f.rdbuf();
        return buffer.str();
    }

    void Renderer::init()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        // Load OpenGL shaders
        std::string fb_vshader = load_source(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/fb_vshader.glsl");
        std::string fb_fshader = load_source(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/fb_fshader.glsl");
        std::string quad_vshader = load_source(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/quad_vshader.glsl");
        std::string quad_fshader = load_source(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/quad_fshader.glsl");
        std::string line_vshader = load_source(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/line_vshader.glsl");
        std::string line_fshader = load_source(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/line_fshader.glsl");
        std::string selection_vshader = load_source(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/selection_vshader.glsl");
        std::string selection_fshader = load_source(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/selection_fshader.glsl");

        // Line shader
        line_shader = std::unique_ptr<OpenGP::Shader>(new OpenGP::Shader());
        line_shader->verbose = true;
        line_shader->add_vshader_from_source(line_vshader.c_str());
        line_shader->add_fshader_from_source(line_fshader.c_str());
        line_shader->link();

        // Bezier control points
        control_points = std::vector<OpenGP::Vec2>();
        control_points.push_back({-0.7f, -0.2f});
        control_points.push_back({-0.3f, 0.2f});
        control_points.push_back({0.3f, 0.5f});
        control_points.push_back({0.7f, 0.0f});

        line = std::unique_ptr<OpenGP::GPUMesh>(new OpenGP::GPUMesh());
        line->set_vbo<OpenGP::Vec2>("vposition", control_points);
        std::vector<unsigned int> indices {0, 1, 2, 3};
        line->set_triangles(indices);

        // Selection shader
        selection_shader = std::unique_ptr<OpenGP::Shader>(new OpenGP::Shader());
        selection_shader->verbose = true;
        selection_shader->add_vshader_from_source(selection_vshader.c_str());
        selection_shader->add_fshader_from_source(selection_fshader.c_str());
        selection_shader->link();

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
        std::vector<unsigned int> quad_triangle_indices {0, 2, 1, 1, 2, 3};
        quad->set_triangles(quad_triangle_indices);
        std::vector<OpenGP::Vec2> quad_vtexcoord {
            {0, 0},
            {0, 1},
            {1, 0},
            {1, 1}
        };
        quad->set_vtexcoord(quad_vtexcoord);

        load_texture(cat,
                     "/Users/michael/Dropbox/Programming/icg/data/nyancat.png");
        load_texture(stars,
                     "/Users/michael/Dropbox/Programming/icg/data/background.png");
    }

    void Renderer::load_texture(std::unique_ptr<OpenGP::RGBA8Texture>& texture,
                                const std::string& filename) const
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
        delete[] row;
        texture = std::unique_ptr<OpenGP::RGBA8Texture>(new OpenGP::RGBA8Texture());
        texture->upload_raw(width, height, &image[0]);
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

        // Framebuffer for selection shader
        OpenGP::Framebuffer selection_framebuffer;
        selection_color = std::unique_ptr<OpenGP::RGBA8Texture>(new OpenGP::RGBA8Texture());
        selection_color->allocate(m_width, m_height);
        selection_depth = std::unique_ptr<OpenGP::D16Texture>(new OpenGP::D16Texture());
        selection_depth->allocate(m_width, m_height);
        selection_framebuffer.attach_color_texture(*selection_color);
        selection_framebuffer.attach_depth_texture(*selection_depth);

        // Mouse position and selected point
        OpenGP::Vec2 pixel_position {0, 0};
        OpenGP::Vec2 position {0, 0};
        OpenGP::Vec2 *selection = nullptr;
        int offset_id = 0;

        OpenGP::RGBA8Texture color_buffer_texture;
        color_buffer_texture.allocate(m_width * 2, m_height * 2);
        OpenGP::Framebuffer framebuffer;
        framebuffer.attach_color_texture(color_buffer_texture);

        OpenGP::Window& window = app.create_window([&](OpenGP::Window& window) {
            glViewport(0, 0, m_width * 2, m_height * 2);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glClear(GL_COLOR_BUFFER_BIT);
            glPointSize(m_point_size);

            framebuffer.bind();
            // glClear(GL_COLOR_BUFFER_BIT);
            draw_scene();
            framebuffer.unbind();

            glClear(GL_COLOR_BUFFER_BIT);
            framebuffer_shader->bind();
            // Bind texture and set uniforms
            glActiveTexture(GL_TEXTURE0);
            color_buffer_texture.bind();
            framebuffer_shader->set_uniform("tex", 0);
            framebuffer_shader->set_uniform("tex_width", float(m_width));
            framebuffer_shader->set_uniform("tex_height", float(m_height));
            quad->set_attributes(*framebuffer_shader);
            quad->draw();
            color_buffer_texture.unbind();
            framebuffer_shader->unbind();

            // Draw line between selection points
            line_shader->bind();
            line_shader->set_uniform("selection", -1);
            line->set_attributes(*line_shader);
            line->set_mode(GL_LINE_STRIP);
            line->draw();

            // Draw selection points
            if (selection != nullptr) {
                line_shader->set_uniform("selection", int(selection - &control_points[0]));
            }
            line->set_mode(GL_POINTS);
            line->draw();
            line_shader->unbind();
        });
        window.set_title("Framebuffer Animation");
        window.set_size(m_width, m_height);

        // Mouse movement callback
        window.add_listener<OpenGP::MouseMoveEvent>([&](const OpenGP::MouseMoveEvent &m) {
            // Mouse position in clip coordinates
            pixel_position = m.position;
            OpenGP::Vec2 centre = {m.position.x() / m_width, -m.position.y() / m_height};
            OpenGP::Vec2 p = 2.0f * (centre - OpenGP::Vec2(0.5f, -0.5f));
            if (selection && (p - position).norm() > 0.0f) {
                selection->x() = position.x();
                selection->y() = position.y();
                line->set_vbo<OpenGP::Vec2>("vposition", control_points);
            }
            position = p;
        });

        // Mouse click callback
        window.add_listener<OpenGP::MouseButtonEvent>([&](const OpenGP::MouseButtonEvent &e) {
            // Mouse selection case
            if (e.button == GLFW_MOUSE_BUTTON_LEFT && !e.released) {
                // Draw element ids to framebuffer
                selection_framebuffer.bind();
                glViewport(0, 0, m_width * 2, m_height * 2);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
                glPointSize(m_point_size);
                selection_shader->bind();
                selection_shader->set_uniform("offset_id", offset_id);
                line->set_attributes(*selection_shader);
                line->set_mode(GL_POINTS);
                line->draw();
                selection_shader->unbind();
                glFlush();
                glFinish();

                selection = nullptr;
                for (auto&& v : control_points) {
                    if ((v - position).norm() < m_point_size / std::min(m_width, m_height)) {
                        selection = &v;
                        break;
                    }
                }
            }
            // Mouse release case
            if (e.button == GLFW_MOUSE_BUTTON_LEFT && e.released) {
                if (selection) {
                    selection->x() = position.x();
                    selection->y() = position.y();
                    selection = nullptr;
                    line->set_vbo<OpenGP::Vec2>("vposition", control_points);
                }
            }
        });

        return app.run();
    }
}
