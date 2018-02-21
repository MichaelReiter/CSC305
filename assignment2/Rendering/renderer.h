#ifndef RENDERER_H
#define RENDERER_H

#include "mesh.h"
#include "OpenGP/GL/Application.h"
#include "OpenGP/GL/Eigen.h"

namespace Rendering {
    class Renderer {
    public:
        Renderer(unsigned int width, unsigned int height, const std::string& mesh_path);

        ~Renderer();

        int create_application();

    private:
        float m_time;
        unsigned int m_width;
        unsigned int m_height;
        std::string m_mesh_path;

        void update();
        std::string read_file_to_string(const std::string& filename) const;
        void read_obj_file(const std::string& filename,
                           std::vector<OpenGP::Vec3>& vertices,
                           std::vector<unsigned int>& indices,
                           std::vector<OpenGP::Vec3>& normals) const;
    };
}

#endif // RENDERER_H
