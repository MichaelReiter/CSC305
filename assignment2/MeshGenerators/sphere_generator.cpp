#include "sphere_generator.h"

namespace MeshGenerators {
    SphereGenerator::SphereGenerator(unsigned int resolution) :
        m_resolution(resolution)
    {}

    SphereGenerator::~SphereGenerator() {}

    void SphereGenerator::write_to_file(std::ofstream& file) const
    {
        file << "# sphere" << std::endl;
        std::vector<OpenGP::Vec3> vertices;
        std::vector<OpenGP::Vec3> faces;
        // Latitude
        int v = 1;
        for (int i = 0; i < m_resolution; i++) {
            float theta1 = ((float)(i) / m_resolution) * M_PI;
            float theta2 = ((float)(i + 1)  / m_resolution) * M_PI;
            // Longitude
            for (int j = 0; j < m_resolution; j++) {
                float phi1 = ((float)(j) / m_resolution) * 2 * M_PI;
                float phi2 = ((float)(j + 1) / m_resolution) * 2 * M_PI;
                vertices.push_back(convert_polar_coordinates_to_cartesian(1.0f, theta1, phi1));
                vertices.push_back(convert_polar_coordinates_to_cartesian(1.0f, theta1, phi2));
                vertices.push_back(convert_polar_coordinates_to_cartesian(1.0f, theta2, phi2));
                vertices.push_back(convert_polar_coordinates_to_cartesian(1.0f, theta2, phi1));
                if (i == 0) {
                    // North pole
                    faces.push_back({v, (v + 3), (v + 2)});
                } else if (i + 1 == m_resolution) {
                    // South pole
                    faces.push_back({(v + 2), (v + 1), v});
                } else {
                    faces.push_back({v, (v + 3), (v + 1)});
                    faces.push_back({(v + 1), (v + 3), (v + 2)});
                }
                v += 4;
            }
        }
        write_vertices(file, vertices);
        write_faces(file, faces);
    }

    OpenGP::Vec3 SphereGenerator::convert_polar_coordinates_to_cartesian(float r, float theta, float phi) const
    {
        float x = r * sinf(theta) * sinf(phi);
        float y = r * cosf(theta);
        float z = r * sinf(theta) * cosf(phi);
        return {x, y, z};
    }
}
