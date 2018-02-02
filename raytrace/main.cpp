#include "bmpwrite.h"
#include "raytracer.h"

int main(int argc, char** argv)
{
    // Antialiased using supersampling
    const int supersample_factor = 2;
    // number of columns (nx)
    const int width_resolution = 1280;
    // number of rows (ny)
    const int height_resolution = 800;

    Raytracer raytracer = {width_resolution, height_resolution, supersample_factor};
    Image<Color> image = raytracer.trace_rays();

    bmpwrite("./out.bmp", image);
    imshow(image);

    return EXIT_SUCCESS;
}
