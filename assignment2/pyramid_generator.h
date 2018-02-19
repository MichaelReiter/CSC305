#ifndef PYRAMID_GENERATOR_H
#define PYRAMID_GENERATOR_H

#include "mesh_generator.h"

class PyramidGenerator : public MeshGenerator {
public:
    PyramidGenerator();

    ~PyramidGenerator();

private:
    void write_to_file(std::ofstream& file) const;
};

#endif // PYRAMID_GENERATOR_H
