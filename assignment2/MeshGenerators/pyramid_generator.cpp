#include "pyramid_generator.h"

namespace MeshGenerators {
    PyramidGenerator::PyramidGenerator() {}

    PyramidGenerator::~PyramidGenerator() {}

    void PyramidGenerator::write_to_file(std::ofstream& file) const
    {
        file << "# pyramid" << std::endl;
        std::vector<OpenGP::Vec3> vertices {
            {0.0f, 0.75f, 0.0f},
            {0.5f, 0.0f, 0.5f},
            {-0.5f, 0.0f, 0.5f},
            {-0.5f, 0.0f, -0.5f},
            {0.5f, 0.0f, -0.5f}
        };
        write_vertices(file, vertices);
        std::vector<OpenGP::Vec3> faces {
            {1, 3, 2},
            {1, 4, 3},
            {1, 5, 4},
            {1, 2, 5},
            {2, 3, 5},
            {3, 4, 5}
        };
        write_faces(file, faces);
    }
}
