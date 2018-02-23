#include "cube_generator.h"

namespace MeshGenerators {
    CubeGenerator::CubeGenerator() {}

    CubeGenerator::~CubeGenerator() {}

    void CubeGenerator::write_to_file(std::ofstream& file) const
    {
        file << "# cube" << std::endl;
        std::vector<OpenGP::Vec3> vertices {
            {-1, -1, 1},
            {1, -1, 1},
            {-1, 1, 1},
            {1, 1, 1},
            {-1, 1, -1},
            {1, 1, -1},
            {-1, -1, -1},
            {1, -1, -1}
        };
        write_vertices(file, vertices);
        std::vector<OpenGP::Vec3> vertex_indices {
            {1, 2, 3},
            {3, 2, 4},
            {3, 4, 5},
            {5, 4, 6},
            {5, 6, 7},
            {7, 6, 8},
            {7, 8, 1},
            {1, 8, 2},
            {2, 8, 4},
            {4, 8, 6},
            {7, 1, 5},
            {5, 1, 3}
        };
        std::vector<OpenGP::Vec3> normals {
            {0, 0, 1},
            {0, 1, 0},
            {0, 0, -1},
            {0, -1, 0},
            {1, 0, 0},
            {-1, 0, 0}
        };
        std::vector<OpenGP::Vec3> normal_indices {
            {1, 1, 1},
            {1, 1, 1},
            {2, 2, 2},
            {2, 2, 2},
            {3, 3, 3},
            {3, 3, 3},
            {4, 4, 4},
            {4, 4, 4},
            {5, 5, 5},
            {5, 5, 5},
            {6, 6, 6},
            {6, 6, 6}
        };
        write_vertex_normals(file, normals);
        write_faces(file, vertex_indices, normal_indices, vertex_indices);
    }
}
