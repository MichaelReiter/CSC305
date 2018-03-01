#include <OpenGP/GL/Application.h>
#include <OpenGP/external/LodePNG/lodepng.cpp>
#include <vector>
#include <string>

using namespace OpenGP;

const int width = 720;
const int height = 720;
typedef Eigen::Transform<float, 3, Eigen::Affine> Transform;

const char* fb_vshader =
#include "fb_vshader.glsl"
;
const char* fb_fshader =
#include "fb_fshader.glsl"
;
const char* quad_vshader =
#include "quad_vshader.glsl"
;
const char* quad_fshader =
#include "quad_fshader.glsl"
;

constexpr float speed_factor = 1.0f;

std::unique_ptr<GPUMesh> quad;
std::unique_ptr<Shader> quadShader;
std::unique_ptr<Shader> framebuffer_shader;
std::unique_ptr<RGBA8Texture> cat;
std::unique_ptr<RGBA8Texture> stars;

// Declare Framebuffer and color buffer texture
std::unique_ptr<Framebuffer> framebuffer;
std::unique_ptr<RGBA8Texture> color_buffer_texture;

void quad_init(std::unique_ptr<GPUMesh>& quad)
{
    quad = std::unique_ptr<GPUMesh>(new GPUMesh());
    std::vector<Vec3> quad_vposition = {
        {-1, -1, 0},
        {-1, 1, 0},
        {1, -1, 0},
        {1, 1, 0}
    };
    quad->set_vbo<Vec3>("vposition", quad_vposition);
    std::vector<unsigned int> quad_triangle_indices = {
        0, 2, 1, 1, 2, 3
    };
    quad->set_triangles(quad_triangle_indices);
    std::vector<Vec2> quad_vtexcoord = {
        {0, 0},
        {0,  1},
        {1, 0},
        {1,  1}
    };
    quad->set_vtexcoord(quad_vtexcoord);
}

void load_texture(std::unique_ptr<RGBA8Texture>& texture, const std::string& filename)
{
    std::vector<unsigned char> image;
    unsigned int width;
    unsigned int height;
    unsigned error = lodepng::decode(image, width, height, filename.c_str());
    if (error) {
        std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    }
    unsigned char* row = new unsigned char[4 * width];
    for (int i = 0; i < int(height) / 2; i++) {
        memcpy(row, &image[4 * i * width], 4 * width * sizeof(unsigned char));
        memcpy(&image[4 * i * width],
               &image[image.size() - 4 * (i + 1) * width],
               4 * width * sizeof(unsigned char));
        memcpy(&image[image.size() - 4 * (i + 1) * width], row, 4 * width * sizeof(unsigned char));
    }
    delete row;
    texture = std::unique_ptr<RGBA8Texture>(new RGBA8Texture());
    texture->upload_raw(width, height, &image[0]);
}

void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    framebuffer_shader = std::unique_ptr<Shader>(new Shader());
    framebuffer_shader->verbose = true;
    framebuffer_shader->add_vshader_from_source(fb_vshader);
    framebuffer_shader->add_fshader_from_source(fb_fshader);
    framebuffer_shader->link();

    quadShader = std::unique_ptr<Shader>(new Shader());
    quadShader->verbose = true;
    quadShader->add_vshader_from_source(quad_vshader);
    quadShader->add_fshader_from_source(quad_fshader);
    quadShader->link();

    quad_init(quad);

    load_texture(cat, "/Users/michael/Dropbox/Programming/icg/build/framebuffer2d/nyancat.png");
    load_texture(stars, "/Users/michael/Dropbox/Programming/icg/build/framebuffer2d/background.png");
}

void draw_scene(float time_count)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float t = time_count * speed_factor;
    Transform transform = Transform::Identity();
    // background.draw(transform.matrix());
    quadShader->bind();
    quadShader->set_uniform("M", transform.matrix());
    // Make texture unit 0 active
    glActiveTexture(GL_TEXTURE0);
    // Bind the texture to the active unit for drawing
    stars->bind();
    // Set the shader's texture uniform to the index of the texture unit we have
    // bound the texture to
    quadShader->set_uniform("tex", 0);
    quad->set_attributes(*quadShader);
    quad->draw();
    stars->unbind();

    float x_coordinate = 0.7f * std::cos(t);
    float y_coordinate = 0.7f * std::sin(t);
    transform *= Eigen::Translation3f(x_coordinate, y_coordinate, 0.0f);
    transform *= Eigen::AngleAxisf(t + M_PI / 2, Eigen::Vector3f::UnitZ());
    transform *= Eigen::AlignedScaling3f(0.2f, 0.2f, 1.0f);

    quadShader->bind();
    quadShader->set_uniform("M", transform.matrix());
    // Make texture unit 0 active
    glActiveTexture(GL_TEXTURE0);
    // Bind the texture to the active unit for drawing
    cat->bind();
    // Set the shader's texture uniform to the index of the texture unit we have bound the texture to
    quadShader->set_uniform("tex", 0);
    quad->set_attributes(*quadShader);
    quad->draw();
    cat->unbind();
    quadShader->unbind();

    glDisable(GL_BLEND);
}

int main(int argc, char** argv)
{
    Application app;
    init();

    // Initialize framebuffer
    framebuffer = std::unique_ptr<Framebuffer>(new Framebuffer());
    // Initialize color buffer texture, and allocate memory
    color_buffer_texture = std::unique_ptr<RGBA8Texture>(new RGBA8Texture());    
    color_buffer_texture->allocate(width, height);

    // Attach color texture to framebuffer
    framebuffer->attach_color_texture(*color_buffer_texture);

    Window& window = app.create_window([](Window&) {
        glViewport(0, 0, width, height);

        // First draw the scene onto framebuffer
        /// Bind and then unbind framebuffer
        framebuffer->bind();
        glClear(GL_COLOR_BUFFER_BIT);
        draw_scene(glfwGetTime());
        framebuffer->unbind();

        /// Render to Window, uncomment the lines and do TODOs
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        framebuffer_shader->bind();
        // Bind texture and set uniforms
        glActiveTexture(GL_TEXTURE0);
        color_buffer_texture->bind();
        framebuffer_shader->set_uniform("tex", 0);
        framebuffer_shader->set_uniform("tex_width", float(width));
        framebuffer_shader->set_uniform("tex_height", float(height));

        quad->set_attributes(*framebuffer_shader);
        quad->draw();
        color_buffer_texture->unbind();
        framebuffer_shader->unbind();
    });
    window.set_title("FrameBuffer");
    window.set_size(width, height);

    return app.run();
}
