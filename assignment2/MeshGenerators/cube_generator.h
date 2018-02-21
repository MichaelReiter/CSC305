#ifndef CUBE_GENERATOR_H
#define CUBE_GENERATOR_H

#include "mesh_generator.h"

namespace MeshGenerators {
    class CubeGenerator : public MeshGenerator {
    public:
        CubeGenerator();

        ~CubeGenerator();

    private:
        void write_to_file(std::ofstream& file) const;
    };
}

#endif // CUBE_GENERATOR_H
