#include "renderer.h"

#include <OpenGP/GL/Application.h>
#include <OpenGP/external/LodePNG/lodepng.cpp>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>

namespace Rendering {
    typedef Eigen::Transform<float, 3, Eigen::Affine> Transform;

    // Textures
    std::unique_ptr<OpenGP::GPUMesh> quad;
    std::unique_ptr<OpenGP::Shader> quad_shader;
    std::unique_ptr<OpenGP::Shader> framebuffer_shader;
    std::unique_ptr<OpenGP::RGBA8Texture> cat;
    std::unique_ptr<OpenGP::RGBA8Texture> background;
    std::unique_ptr<OpenGP::RGBA8Texture> tail_up;
    std::unique_ptr<OpenGP::RGBA8Texture> tail_down;
    std::unique_ptr<OpenGP::Framebuffer> framebuffer;
    std::unique_ptr<OpenGP::RGBA8Texture> color_buffer_texture;

    // Selection
    std::unique_ptr<OpenGP::GPUMesh> line;
    std::unique_ptr<OpenGP::GPUMesh> bezier_curve;
    std::unique_ptr<OpenGP::Shader> line_shader;
    std::vector<OpenGP::Vec2> control_points;
    std::vector<OpenGP::Vec2> bezier_points;
    std::unique_ptr<OpenGP::Shader> selection_shader;
    std::unique_ptr<OpenGP::RGBA8Texture> selection_color;
    std::unique_ptr<OpenGP::D16Texture> selection_depth;

    Renderer::Renderer(unsigned int width,
                       unsigned int height,
                       float speed_factor,
                       int bezier_resolution,
                       float point_size) :
        m_width(width),
        m_height(height),
        m_speed_factor(speed_factor),
        m_bezier_resolution(bezier_resolution),
        m_point_size(point_size),
        m_time(0.0f)
    {}

    Renderer::~Renderer() {}

    std::string Renderer::load_shader(const std::string& filename) const
    {
        std::ifstream f(filename.c_str());
        std::stringstream buffer;
        buffer << f.rdbuf();
        return buffer.str();
    }

    void Renderer::compute_and_load_bezier_points()
    {
        std::vector<unsigned int> bezier_indices;
        bezier_points = std::vector<OpenGP::Vec2>();
        // The Bezier curve is defined by lines connection m_bezier_resolution points
        for (int i = 0; i < m_bezier_resolution + 1; i++) {
            float t = (float)i / m_bezier_resolution;
            OpenGP::Vec2 b = std::pow((1 - t), 3) * control_points[0]
                           + std::pow((1 - t), 2) * control_points[1] * 3 * t
                           + (1 - t) * control_points[2] * std::pow(t, 2)
                           + std::pow(t, 3) * control_points[3];
            bezier_points.push_back(b);
            bezier_indices.push_back(i);
        }
        bezier_curve->set_vbo<OpenGP::Vec2>("vposition", bezier_points);
        bezier_curve->set_triangles(bezier_indices);
    }

    void Renderer::init()
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        // Load OpenGL shaders
        std::string fb_vshader = load_shader(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/fb_vshader.glsl");
        std::string fb_fshader = load_shader(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/fb_fshader.glsl");
        std::string quad_vshader = load_shader(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/quad_vshader.glsl");
        std::string quad_fshader = load_shader(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/quad_fshader.glsl");
        std::string line_vshader = load_shader(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/line_vshader.glsl");
        std::string line_fshader = load_shader(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/line_fshader.glsl");
        std::string selection_vshader = load_shader(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/selection_vshader.glsl");
        std::string selection_fshader = load_shader(
            "/Users/michael/Dropbox/Programming/icg/animation2d/Shaders/selection_fshader.glsl");

        // Framebuffer for selection shader
        color_buffer_texture = std::unique_ptr<OpenGP::RGBA8Texture>(new OpenGP::RGBA8Texture());
        color_buffer_texture->allocate(m_width * 2, m_height * 2);
        framebuffer = std::unique_ptr<OpenGP::Framebuffer>(new OpenGP::Framebuffer());
        framebuffer->attach_color_texture(*color_buffer_texture);
        
        OpenGP::Framebuffer selection_framebuffer;
        selection_color = std::unique_ptr<OpenGP::RGBA8Texture>(new OpenGP::RGBA8Texture());
        selection_color->allocate(m_width, m_height);
        selection_depth = std::unique_ptr<OpenGP::D16Texture>(new OpenGP::D16Texture());
        selection_depth->allocate(m_width, m_height);
        selection_framebuffer.attach_color_texture(*selection_color);
        selection_framebuffer.attach_depth_texture(*selection_depth);

        // Line shader
        line_shader = std::unique_ptr<OpenGP::Shader>(new OpenGP::Shader());
        line_shader->verbose = true;
        line_shader->add_vshader_from_source(line_vshader.c_str());
        line_shader->add_fshader_from_source(line_fshader.c_str());
        line_shader->link();

        // Bezier control points
        control_points = std::vector<OpenGP::Vec2>();
        control_points.push_back({-0.75f, -0.75f});
        control_points.push_back({-0.75f, 0.75f});
        control_points.push_back({0.75f, -0.75f});
        control_points.push_back({0.75f, 0.75f});

        // Line between control points
        line = std::unique_ptr<OpenGP::GPUMesh>(new OpenGP::GPUMesh());
        line->set_vbo<OpenGP::Vec2>("vposition", control_points);
        std::vector<unsigned int> line_indices {0, 1, 2, 3};
        line->set_triangles(line_indices);

        // Bezier points
        bezier_curve = std::unique_ptr<OpenGP::GPUMesh>(new OpenGP::GPUMesh());
        compute_and_load_bezier_points();

        // Selection shader
        selection_shader = std::unique_ptr<OpenGP::Shader>(new OpenGP::Shader());
        selection_shader->verbose = true;
        selection_shader->add_vshader_from_source(selection_vshader.c_str());
        selection_shader->add_fshader_from_source(selection_fshader.c_str());
        selection_shader->link();

        // Framebuffer shader
        framebuffer_shader = std::unique_ptr<OpenGP::Shader>(new OpenGP::Shader());
        framebuffer_shader->verbose = true;
        framebuffer_shader->add_vshader_from_source(fb_vshader.c_str());
        framebuffer_shader->add_fshader_from_source(fb_fshader.c_str());
        framebuffer_shader->link();

        // Quad shader
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

        // Load textures to GPU
        load_texture(cat,
                     "/Users/michael/Dropbox/Programming/icg/data/nyancat_no_tail.png");
        load_texture(background,
                     "/Users/michael/Dropbox/Programming/icg/data/background.png");
        load_texture(tail_up,
                     "/Users/michael/Dropbox/Programming/icg/data/rainbow_tail_up.png");
        load_texture(tail_down,
                     "/Users/michael/Dropbox/Programming/icg/data/rainbow_tail_down.png");
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

    void Renderer::draw_textures()
    {
        // Enable alpha blending for texture transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Draw background
        quad_shader->bind();
        quad_shader->set_uniform("M", Transform::Identity().matrix());
        glActiveTexture(GL_TEXTURE0);
        background->bind();
        quad_shader->set_uniform("tex", 0);
        quad->set_attributes(*quad_shader);
        quad->draw();
        background->unbind();

        // Cat transformations
        Transform cat_model = Transform::Identity();
        float t = M_PI * m_time * m_speed_factor;
        int index = (int)t % m_bezier_resolution;
        constexpr float cat_scale = 0.15f;
        // Translate cat along bezier curve
        cat_model *= Eigen::Translation3f(bezier_points[index].x(), bezier_points[index].y(), 0.0f);
        // Scale cat
        cat_model *= Eigen::AlignedScaling3f(cat_scale, cat_scale, 1.0f);

        // Tail transformations
        Transform tail_model = cat_model;
        // Translate tail to the right (behind cat)
        tail_model *= Eigen::Translation3f(0.5f, 0.0f, 0.0f);
        // Scale the tail
        float tail_scale_y = 0.1f * std::sinf(t) + 1.0f;
        float tail_scale_x = tail_scale_y * 2;
        tail_model *= Eigen::AlignedScaling3f(tail_scale_x, tail_scale_y, 1.0f);

        // Draw tail
        quad_shader->bind();
        quad_shader->set_uniform("M", tail_model.matrix());
        glActiveTexture(GL_TEXTURE0);
        if ((int)(m_time * 100) % 2 == 0) {
            tail_up->bind();
        } else {
            tail_down->bind();
        }
        quad_shader->set_uniform("tex", 0);
        quad->set_attributes(*quad_shader);
        quad->draw();
        if ((int)(m_time * 100) % 2 == 0) {
            tail_up->unbind();
        } else {
            tail_down->unbind();
        }
        quad_shader->unbind();
        
        // Draw cat
        quad_shader->bind();
        quad_shader->set_uniform("M", cat_model.matrix());
        glActiveTexture(GL_TEXTURE0);
        cat->bind();
        quad_shader->set_uniform("tex", 0);
        quad->set_attributes(*quad_shader);
        quad->draw();
        cat->unbind();
        quad_shader->unbind();

        glDisable(GL_BLEND);
    }

    void Renderer::update()
    {
        m_time += 0.001f;
    }

    int Renderer::create_application()
    {
        OpenGP::Application app;
        init();

        // Mouse position and selected point
        OpenGP::Vec2 position {0, 0};
        OpenGP::Vec2* selection = nullptr;

        app.add_listener<OpenGP::ApplicationUpdateEvent>(
            [this](const OpenGP::ApplicationUpdateEvent& aue) {
                update();
            }
        );

        OpenGP::Window& window = app.create_window([&](OpenGP::Window& window) {
            glViewport(0, 0, m_width * 2, m_height * 2);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glClear(GL_COLOR_BUFFER_BIT);
            glPointSize(m_point_size);

            framebuffer->bind();
            draw_textures();
            framebuffer->unbind();

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

            line_shader->bind();
            // Draw Bezier curve
            line_shader->set_uniform("selection", -1);
            bezier_curve->set_attributes(*line_shader);
            bezier_curve->set_mode(GL_LINE_STRIP);
            bezier_curve->draw();
            // Draw lines between selection points
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
        window.set_title("Animation");
        window.set_size(m_width, m_height);

        // Mouse movement callback
        window.add_listener<OpenGP::MouseMoveEvent>([&](const OpenGP::MouseMoveEvent& m) {
            // Mouse position in clip coordinates
            OpenGP::Vec2 centre = {m.position.x() / m_width, -m.position.y() / m_height};
            OpenGP::Vec2 p = 2.0f * (centre - OpenGP::Vec2(0.5f, -0.5f));
            if (selection && (p - position).norm() > 0.0f) {
                selection->x() = position.x();
                selection->y() = position.y();
                compute_and_load_bezier_points();
                line->set_vbo<OpenGP::Vec2>("vposition", control_points);
            }
            position = p;
        });

        // Mouse click callback
        window.add_listener<OpenGP::MouseButtonEvent>([&](const OpenGP::MouseButtonEvent& e) {
            // Mouse down
            if (e.button == GLFW_MOUSE_BUTTON_LEFT && !e.released) {
                selection = nullptr;
                for (auto&& v : control_points) {
                    if ((v - position).norm() < m_point_size / std::min(m_width, m_height)) {
                        selection = &v;
                        break;
                    }
                }
            }
            // Mouse up
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
