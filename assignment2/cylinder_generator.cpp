#include "cylinder_generator.h"

CylinderGenerator::CylinderGenerator(unsigned int resolution) :
    m_resolution(resolution)
{}

CylinderGenerator::~CylinderGenerator() {}

void CylinderGenerator::write_to_file(std::ofstream& file) const
{
    file << "# cylinder" << std::endl;
    std::vector<OpenGP::Vec3> vertices;
    // Points around circles
    for (int i = 0; i < m_resolution; i++) {
        float theta = (2 * M_PI * i) / (m_resolution - 1);
        vertices.push_back({sin(theta), 1.0f, cos(theta)});
        vertices.push_back({sin(theta), -1.0f, cos(theta)});
    }
    // North and South poles in the center of the circles
    vertices.push_back({0.0f, 1.0f, 0.0f});
    vertices.push_back({0.0f, -1.0f, 0.0f});
    write_vertices(file, vertices);
    std::vector<OpenGP::Vec3> faces;
    // Faces on the rectangle
    for (int i = 1; i < (2 * m_resolution); i++) {
        faces.push_back({(i), (i + 1), (i + 2)});
        faces.push_back({(i + 1), (i + 3), (i + 2)});
    }
    // Faces on the circles
    for (int i = 1; i < (2 * m_resolution - 1); i++) {
        if (i % 2 == 0) {
            faces.push_back({i, (2 * m_resolution + 2), (i + 2)});
        } else {
            faces.push_back({(i + 2), (2 * m_resolution + 1), i});
        }
    }
    write_faces(file, faces);
}
