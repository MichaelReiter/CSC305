#include "cube_generator.h"

CubeGenerator::CubeGenerator() {}

CubeGenerator::~CubeGenerator() {}

void CubeGenerator::write_to_file(std::ofstream& file) const
{
    file << "# cube" << std::endl;
    std::vector<OpenGP::Vec3> vertices {
        {-1, 1, -1},
        {-1, 1, 1},
        {1, 1, 1},
        {1, 1, -1},
        {-1, -1, -1},
        {-1, -1, 1},
        {1, -1, 1},
        {1, -1, -1}
    };
    write_vertices(file, vertices);
    std::vector<OpenGP::Vec3> faces {
        {6, 2, 1},
        {6, 1, 5},
        {8, 4, 3},
        {8, 3, 7},
        {8, 7, 6},
        {8, 6, 5},
        {3, 4, 1},
        {3, 1, 2},
        {5, 1, 4},
        {5, 4, 8},
        {7, 3, 2},
        {7, 2, 6}
    };
    write_faces(file, faces);
}
