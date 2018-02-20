#ifndef RENDERER_H
#define RENDERER_H

#include "OpenGP/GL/Application.h"
#include "OpenGP/GL/Eigen.h"
#include "../Mesh/mesh.h"

class Renderer {
public:
    Renderer(unsigned int width, unsigned int height);

    ~Renderer();

    int create_application();

private:
    float m_time;
    unsigned int m_width;
    unsigned int m_height;

    void update();
    std::string read_file_to_string(const std::string& filename) const;
    void draw(OpenGP::Shader& shader, Mesh& mesh) const;
};

#endif // RENDERER_H
