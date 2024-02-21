#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "Color.h"
#include "Ray.h"
#include "Vec3.h"

Color RayColor(const Ray& ray)
{
    Vec3 unitDirection = UnitVector(ray.Direction());
    double a = 0.5 * (unitDirection.Y() + 1.0);
    return (1 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int main()
{
    constexpr double aspectRatio = 16.0 / 9.0;

    constexpr int imageWidth = 400;
    constexpr int imageHeight = std::max(1, static_cast<int>(imageWidth / aspectRatio));

    double focalLength = 1.0;

    char* buffer = new char[imageWidth * imageHeight * 3];
    for (int j = imageHeight - 1; j >= 0; --j)
    {
        std::clog << "Scan lines remaining: " << j <<  '\n';
        for (int i = 0; i < imageWidth; ++i)
        {
            double r = double(i) / (imageWidth - 1);
            double g = double(j) / (imageHeight - 1);
            double b = 0.25;

            int index = (j * imageWidth + i) * 3;
            buffer[index] = static_cast<char>(255.999 * r);
            buffer[index + 1] = static_cast<char>(255.999 * g);
            buffer[index + 2] = static_cast<char>(255.999 * b);
        }
    }

    stbi_write_jpg("image.jpg", imageWidth, imageHeight, 3, buffer, 100);

    std::clog << "\nDone\n";

    delete[] buffer;
    return 0;
}
