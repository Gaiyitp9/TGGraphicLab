#pragma once

#include "Vec3.h"
#include "Scene.h"

class Camera
{
public:
    Camera();
    Camera(double aspectRatio, int imageWidth);

    void Update();

    void Render(char const* imageFile, const Scene& scene);

private:
    Ray GetRay(int row, int column);

public:
    double          aspectRatio = 16.0 / 9.0;
    unsigned int    imageWidth = 400;
    unsigned int    samplePerPixel = 100;
    unsigned int    maxDepth = 50;

private:
    double          m_focalLength = 1.0;
    unsigned int    m_imageHeight = 225;
    double          m_viewportWidth = 32.0 / 9.0;
    double          m_viewportHeight = 2.0;

    Point3  m_position = Vec3(0, 0, 0);
    Vec3    m_viewportU = Vec3(m_viewportWidth, 0, 0);
    Vec3    m_viewportV = Vec3(0, -m_viewportHeight, 0);
    Vec3    m_pixelDeltaU = m_viewportU / imageWidth;
    Vec3    m_pixelDeltaV = m_viewportV / m_imageHeight;
    Point3  m_viewportUpperLeft = m_position - Vec3(0.0, 0.0, m_focalLength) - 0.5 * (m_viewportU + m_viewportV);
    Point3  m_pixel00Loc = m_viewportUpperLeft + 0.5 * (m_pixelDeltaU + m_pixelDeltaV);
};
