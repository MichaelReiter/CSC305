#include "Mesh/Mesh.h"
#include "OpenGP/GL/glfw_helpers.h"
#include <OpenGP/types.h>
#include <OpenGP/MLogger.h>
#include <OpenGP/GL/Application.h>
#include <OpenGP/GL/ImguiRenderer.h>

using namespace OpenGP;

int main()
{
    Application app;
    ImguiRenderer imgui_renderer;
    Mesh render_mesh;

    // Example rendering a mesh
    // Call to compile shaders
    render_mesh.init();

    // Load Vertices and Indices (minimum required for Mesh::draw to work)
    std::vector<Vec3> vertex_list;
    vertex_list.push_back(Vec3(0,0,0));
    vertex_list.push_back(Vec3(1,0,0));
    vertex_list.push_back(Vec3(1,1,0));
    vertex_list.push_back(Vec3(0,1,0));
    std::vector<unsigned int> index_list;
    index_list.push_back(0); // Face 1
    index_list.push_back(1);
    index_list.push_back(2);
    index_list.push_back(2); // Face 2
    index_list.push_back(3);
    index_list.push_back(0);
    render_mesh.load_vertices(vertex_list, index_list);

    // Load normals
    std::vector<Vec3> normal_list;
    normal_list.push_back(Vec3(0,0,1));
    normal_list.push_back(Vec3(0,0,1));
    normal_list.push_back(Vec3(0,0,1));
    normal_list.push_back(Vec3(0,0,1));
    render_mesh.load_normals(normal_list);

    // Load textures (assumes texcoords)
    render_mesh.load_textures("/Users/michael/Dropbox/Programming/icg/build/assignment2/earth.png");

    // Load texture coordinates (assumes textures)
    std::vector<Vec2> texture_coordinate_list;
    texture_coordinate_list.push_back(Vec2(0,0));
    texture_coordinate_list.push_back(Vec2(1,0));
    texture_coordinate_list.push_back(Vec2(1,1));
    texture_coordinate_list.push_back(Vec2(0,1));
    render_mesh.load_texture_coordinates(texture_coordinate_list);

    // Create main window, set callback function
    auto &window1 = app.create_window([&](Window &window) {
        int width;
        int height;
        std::tie(width, height) = window.get_size();

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Wireframe rendering, might be helpful when debugging your mesh generation
        // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        float ratio = width / (float) height;
        Mat4x4 model_transform = Mat4x4::Identity();
        Mat4x4 model = model_transform.matrix();
        Mat4x4 projection = OpenGP::perspective(70.0f, ratio, 0.1f, 10.0f);

        //camera movement
        float time = 0.5f * (float)glfwGetTime();
        Vec3 cam_pos(2 * cos(time), 2.0, 2 * sin(time));
        Vec3 cam_look(0.0f, 0.0f, 0.0f);
        Vec3 cam_up(0.0f, 1.0f, 0.0f);
        Mat4x4 view = OpenGP::lookAt(cam_pos, cam_look, cam_up);

        render_mesh.draw(model, view, projection);
    });
    window1.set_title("Assignment 2");

    // // Create window for IMGUI, set callback function
    // auto &window2 = app.create_window([&](Window &window) {
    //     int width;
    //     int height;
    //     std::tie(width, height) = window.get_size();

    //     imgui_renderer.begin_frame(width, height);

    //     ImGui::BeginMainMenuBar();
    //     ImGui::MenuItem("File");
    //     ImGui::MenuItem("Edit");
    //     ImGui::MenuItem("View");
    //     ImGui::MenuItem("Help");
    //     ImGui::EndMainMenuBar();

    //     ImGui::Begin("Test Window 1");
    //     ImGui::Text("This is a test imgui window");
    //     ImGui::End();

    //     glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    //     glClear(GL_COLOR_BUFFER_BIT);

    //     imgui_renderer.end_frame();
    // });
    // window2.set_title("imgui Test");

    return app.run();
}
