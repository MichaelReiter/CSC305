#include "bmpwrite.h"
#include "raytracer.h"

int main(int argc, char** argv)
{
    // Antialiased using supersampling
    constexpr int supersample_factor = 2;
    // number of columns (nx)
    constexpr int width_resolution = 1280;
    // number of rows (ny)
    constexpr int height_resolution = 800;

    Raytracer raytracer = {width_resolution, height_resolution, supersample_factor};
    Image image = raytracer.trace_rays();

    bmpwrite("./out.bmp", image);
    OpenGP::imshow(image);

    return EXIT_SUCCESS;
}
