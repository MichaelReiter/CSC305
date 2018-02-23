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
        std::vector<OpenGP::Vec3> normals;
        std::vector<OpenGP::Vec3> vertex_indices;
        std::vector<OpenGP::Vec2> texture_coordinates;
        OpenGP::Vec3 origin = {0.0f, 0.0f, 0.0f};
        int v = 1;
        // Latitude
        for (int i = 0; i < m_resolution; i++) {
            float theta1 = ((float)(i) / m_resolution) * M_PI;
            float theta2 = ((float)(i + 1)  / m_resolution) * M_PI;
            // Longitude
            for (int j = 0; j < m_resolution+1; j++) {
                float phi1 = ((float)(j) / m_resolution) * 2 * M_PI;
                float phi2 = ((float)(j + 1) / m_resolution) * 2 * M_PI;
                OpenGP::Vec3 v1 = convert_polar_coordinates_to_cartesian(1.0f, theta1, phi1);
                OpenGP::Vec3 v2 = convert_polar_coordinates_to_cartesian(1.0f, theta1, phi2);
                OpenGP::Vec3 v3 = convert_polar_coordinates_to_cartesian(1.0f, theta2, phi2);
                OpenGP::Vec3 v4 = convert_polar_coordinates_to_cartesian(1.0f, theta2, phi1);
                OpenGP::Vec3 d1 = (v1 - origin).normalized();
                OpenGP::Vec3 d2 = (v2 - origin).normalized();
                OpenGP::Vec3 d3 = (v3 - origin).normalized();
                OpenGP::Vec3 d4 = (v4 - origin).normalized();
                vertices.push_back(v1);
                vertices.push_back(v2);
                vertices.push_back(v3);
                vertices.push_back(v4);
                normals.push_back(d1);
                normals.push_back(d2);
                normals.push_back(d3);
                normals.push_back(d4);
                texture_coordinates.push_back({(atan2(d1[0], d1[2]) / (2 * M_PI) + 0.5), d1[1] * 0.5 + 0.5});
                texture_coordinates.push_back({(atan2(d2[0], d2[2]) / (2 * M_PI) + 0.5), d2[1] * 0.5 + 0.5});
                texture_coordinates.push_back({(atan2(d3[0], d3[2]) / (2 * M_PI) + 0.5), d3[1] * 0.5 + 0.5});
                texture_coordinates.push_back({(atan2(d4[0], d4[2]) / (2 * M_PI) + 0.5), d4[1] * 0.5 + 0.5});
                if (i == 0) {
                    // North pole
                    vertex_indices.push_back({v, (v + 3), (v + 2)});
                } else if (i + 1 == m_resolution) {
                    // South pole
                    vertex_indices.push_back({(v + 2), (v + 1), (v)});
                } else {
                    vertex_indices.push_back({v, (v + 3), (v + 1)});
                    vertex_indices.push_back({(v + 1), (v + 3), (v + 2)});
                }
                v += 4;
            }
        }
        write_vertices(file, vertices);
        write_vertex_normals(file, normals);
        write_texture_coordinates(file, texture_coordinates);
        write_faces(file, vertex_indices, vertex_indices, vertex_indices);
    }

    OpenGP::Vec3 SphereGenerator::convert_polar_coordinates_to_cartesian(float r, float theta, float phi) const
    {
        float x = r * sinf(theta) * sinf(phi);
        float y = r * cosf(theta);
        float z = r * sinf(theta) * cosf(phi);
        return {x, y, z};
    }
}
