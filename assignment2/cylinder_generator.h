#ifndef CYLINDER_GENERATOR_H
#define CYLINDER_GENERATOR_H

#include "mesh_generator.h"

class CylinderGenerator : public MeshGenerator {
public:
    CylinderGenerator(unsigned int resolution);

    ~CylinderGenerator();

private:
    unsigned int m_resolution;

    void write_to_file(std::ofstream& file) const;
};

#endif // CYLINDER_GENERATOR_H
