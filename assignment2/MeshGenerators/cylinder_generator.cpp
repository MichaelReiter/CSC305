#include "cylinder_generator.h"

namespace MeshGenerators {
    CylinderGenerator::CylinderGenerator(unsigned int resolution) :
        m_resolution(resolution)
    {}

    CylinderGenerator::~CylinderGenerator() {}

    void CylinderGenerator::write_to_file(std::ofstream& file) const
    {
        file << "# cylinder" << std::endl;
        OpenGP::Vec3 north_pole = {0.0f, 1.0f, 0.0f};
        OpenGP::Vec3 south_pole = {0.0f, -1.0f, 0.0f};
        std::vector<OpenGP::Vec3> vertices;
        std::vector<OpenGP::Vec3> normals;
        // Points around circles
        for (int i = 0; i < m_resolution; i++) {
            float theta = (2 * M_PI * i) / (m_resolution - 1);
            OpenGP::Vec3 v1 = {sin(theta), 1.0f, cos(theta)};
            OpenGP::Vec3 v2 = {sin(theta), -1.0f, cos(theta)};
            vertices.push_back(v1);
            vertices.push_back(v2);
            normals.push_back(v1 - north_pole);
            normals.push_back(v2 - south_pole);
        }
        // North and South poles in the center of the circles
        vertices.push_back({0.0f, 1.0f, 0.0f});
        vertices.push_back({0.0f, -1.0f, 0.0f});
        normals.push_back({0.0f, 1.0f, 0.0f});
        normals.push_back({0.0f, -1.0f, 0.0f});
        write_vertices(file, vertices);
        write_vertex_normals(file, normals);
        std::vector<OpenGP::Vec3> vertex_indices;
        // Faces on the rectangle
        for (int i = 1; i < (2 * m_resolution - 2); i++) {
            vertex_indices.push_back({(i), (i + 1), (i + 2)});
            vertex_indices.push_back({(i + 1), (i + 3), (i + 2)});
        }
        // Faces on the circles
        for (int i = 1; i < (2 * m_resolution - 1); i++) {
            if (i % 2 == 0) {
                vertex_indices.push_back({i, (2 * m_resolution + 2), (i + 2)});
            } else {
                vertex_indices.push_back({(i + 2), (2 * m_resolution + 1), i});
            }
        }
        write_faces(file, vertex_indices, vertex_indices);
    }
}
