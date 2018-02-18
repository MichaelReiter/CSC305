#ifndef SPHERE_GENERATOR_H
#define SPHERE_GENERATOR_H

#include "mesh_generator.h"

class SphereGenerator : public MeshGenerator {
public:
    SphereGenerator(unsigned int resolution);

    ~SphereGenerator();

private:
    unsigned int m_resolution;

    void write_to_file(std::ofstream& file) const;
};

#endif // SPHERE_GENERATOR_H
