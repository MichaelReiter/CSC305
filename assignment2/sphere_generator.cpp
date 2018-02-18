#include <iostream>
#include <math.h>
#include <vector>
#include "sphere_generator.h"

SphereGenerator::SphereGenerator(unsigned int resolution) :
    m_resolution(resolution)
{}

SphereGenerator::~SphereGenerator() {}

void SphereGenerator::write_to_file(std::ofstream& file) const
{
    file << "# sphere" << std::endl;
    std::vector<OpenGP::Vec3> vertices;

    // vertices.push_back({});
    write_vertices(file, vertices);
    std::vector<OpenGP::Vec3> faces;

    // faces.push_back({});
    write_faces(file, faces);
}
