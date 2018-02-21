#include "cone_generator.h"

namespace MeshGenerators {
    ConeGenerator::ConeGenerator(unsigned int resolution) :
        m_resolution(resolution)
    {}

    ConeGenerator::~ConeGenerator() {}

    void ConeGenerator::write_to_file(std::ofstream& file) const
    {
        file << "# cone" << std::endl;
        std::vector<OpenGP::Vec3> vertices;
        // Points around circles
        for (int i = 0; i < m_resolution; i++) {
            float theta = (2 * M_PI * i) / (m_resolution - 1);
            vertices.push_back({sin(theta), -1.0f, cos(theta)});
        }
        // North and South poles
        vertices.push_back({0.0f, 0.5f, 0.0f});
        vertices.push_back({0.0f, -1.0f, 0.0f});
        write_vertices(file, vertices);
        std::vector<OpenGP::Vec3> faces;
        // Faces on the spike
        for (int i = 1; i < m_resolution; i++) {
            faces.push_back({i, i + 1, m_resolution + 1});
        }
        // Faces on the circle
        for (int i = 1; i < m_resolution; i++) {
            faces.push_back({i, (m_resolution + 2), (i + 1)});
        }
        write_faces(file, faces);
    }
}
