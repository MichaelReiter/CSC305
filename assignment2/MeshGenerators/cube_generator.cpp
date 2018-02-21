#include "cube_generator.h"

namespace MeshGenerators {
    CubeGenerator::CubeGenerator() {}

    CubeGenerator::~CubeGenerator() {}

    void CubeGenerator::write_to_file(std::ofstream& file) const
    {
        file << "# cube" << std::endl;
        std::vector<OpenGP::Vec3> vertices {
            {-1, -1, 1},
            {-1, 1, 1},
            {1, 1, 1},
            {1, -1, 1},
            {-1, -1, -1},
            {-1, 1, -1},
            {1, 1, -1},
            {1, -1, -1}
        };
        write_vertices(file, vertices);
        std::vector<OpenGP::Vec3> faces {
            {6, 1, 2},
            {6, 5, 1},
            {8, 3, 4},
            {8, 7, 3},
            {8, 6, 7},
            {8, 5, 6},
            {3, 1, 4},
            {3, 2, 1},
            {5, 4, 1},
            {5, 8, 4},
            {7, 2, 3},
            {7, 6, 2}
        };
        write_faces(file, faces);
    }
}
