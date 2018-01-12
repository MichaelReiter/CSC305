#include "OpenGP/Image/Image.h"
#include "bmpwrite.h"

using namespace OpenGP;

using Colour = Vec3; // RGB Value
Colour red() { return Colour(1.0f, 0.0f, 0.0f); }
Colour white() { return Colour(1.0f, 1.0f, 1.0f); }
Colour black() { return Colour(0.0f, 0.0f, 0.0f); }

int main(int, char**) {
    // number of columns (nx)
    int widthResolution = 800;

    // number of rows (ny)
    int heightResolution = 600;

    Image<Colour> image(heightResolution, widthResolution);

    // e
    Vec3 cameraPosition = Vec3(0, 0, 0);

    Vec3 spherePosition = Vec3(0, 5, 0);

    for (int row = 0; row < image.rows(); row++) {
        for (int col = 0; col < image.cols(); col++) {
            /// TODO: build primary rays
            // example: Vec3 o = Vec3(0,0,0);

            // Vec3 ray = Vec3(0,0,0);

            // Compute u and v
            // u = l + (r − l) * (i + 0.5) / nx
            // v = b + (t − b) * (j + 0.5) / ny

            // Compute ray direction
            // d is image plane distance (i.e. focal length)
            // ray direction = −d*w + u*u + v*v

            // ray origin is camera position (e)
            
            /// TODO: ray sphere intersection and shading

            /// EXAMPLE: use "image(row, col) = colour;" to set pixel values
            // if (row > 0 && row < 200 && col > 0 && col < 200)  {
            //     image(row, col) = red();
            // } else {
            //     image(row, col) = white();
            // }
            image(row, col) = white();
        }
    }

    bmpwrite("./out.bmp", image);
    imshow(image);

    return EXIT_SUCCESS;
}
