#include "Camera.h"
// #include "stb/stb_image_write.h"
#include "rtweekend.h"

Camera::Camera() = default;

Camera::Camera(double aspectRatio, int imageWidth)
        : m_focalLength(1.0), aspectRatio(aspectRatio), imageWidth(imageWidth), m_viewportHeight(2.0),
          m_position(0.0, 0.0, 0.0)
{
    Update();
}

void Camera::Update()
{
    m_imageHeight = std::max(1, static_cast<int>(imageWidth / aspectRatio));
    m_viewportWidth = m_viewportHeight * static_cast<double>(imageWidth) / m_imageHeight;
    m_viewportU = Vec3(m_viewportWidth, 0, 0);
    m_viewportV = Vec3(0, -m_viewportHeight, 0);
    m_pixelDeltaU = m_viewportU / imageWidth;
    m_pixelDeltaV = m_viewportV / m_imageHeight;
    m_viewportUpperLeft = m_position - Vec3(0.0, 0.0, m_focalLength) - 0.5 * (m_viewportU + m_viewportV);
    m_pixel00Loc = m_viewportUpperLeft + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);
}

void Camera::Render(char const* imageFile, const Scene& scene)
{
    char* buffer = new char[imageWidth * m_imageHeight * 3];
    for (int i = 0; i < m_imageHeight; ++i)
    {
        std::clog << "Scan lines remaining: " << m_imageHeight - i <<  '\n';
        for (int j = 0; j < imageWidth; ++j)
        {
            Color pixelColor;
            // 抗锯齿
            for (int sample = 0; sample < samplePerPixel; ++sample)
            {
                Ray ray = GetRay(i, j);
                pixelColor += scene.RayColor(ray, maxDepth);
            }
            pixelColor /= samplePerPixel;
            pixelColor.SetX(Linear2Gamma(pixelColor.X()));
            pixelColor.SetY(Linear2Gamma(pixelColor.Y()));
            pixelColor.SetZ(Linear2Gamma(pixelColor.Z()));

            Interval intensity(0.0, 1.0);
            unsigned int index = (i * imageWidth + j) * 3;
            buffer[index] = static_cast<char>(255.999 * intensity.Clamp(pixelColor.X()));
            buffer[index + 1] = static_cast<char>(255.999 * intensity.Clamp(pixelColor.Y()));
            buffer[index + 2] = static_cast<char>(255.999 * intensity.Clamp(pixelColor.Z()));
        }
    }

    // stbi_write_jpg(imageFile, static_cast<int>(imageWidth), static_cast<int>(m_imageHeight), 3, buffer, 100);

    std::clog << "\nDone\n";

    delete[] buffer;
}

Ray Camera::GetRay(int row, int column)
{
    Point3 pixelCenter = m_pixel00Loc + row * m_pixelDeltaV + column * m_pixelDeltaU;
    double px = -0.5 + RandomDouble();
    double py = -0.5 + RandomDouble();
    // 在(row, column)像素内随机取一个采样点
    Point3 pixelSample = pixelCenter + px * m_pixelDeltaU + py * m_pixelDeltaV;
    Vec3 rayDirection = pixelSample - m_position;
    return {m_position, rayDirection};
}
