#ifndef CONE_GENERATOR_H
#define CONE_GENERATOR_H

#include "mesh_generator.h"

namespace MeshGenerators {
    class ConeGenerator : public MeshGenerator {
    public:
        ConeGenerator(unsigned int resolution);

        ~ConeGenerator();

    private:
        unsigned int m_resolution;

        void write_to_file(std::ofstream& file) const;
    };
}

#endif // CONE_GENERATOR_H
