/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/RayTracing/Camera.h"
#include "Rendering/Color/Color.h"
#include "stb_image_write.h"

namespace TG::Rendering::RayTracing
{
    Camera::Camera(unsigned int imageWidth, Math::Vector3f cameraPosition, float aspectRatio, float focalLength,
        float fov, Math::Vector3f front, Math::Vector3f up, Math::Vector3f right, unsigned int samplesPerPixel,
        unsigned int maxDepth) :
            m_imageWidth(imageWidth),
            m_imageHeight(std::max(1, static_cast<int>(static_cast<float>(m_imageWidth) / aspectRatio))),
            m_position(cameraPosition),
            m_front(front),
            m_up(up),
            m_right(right),
            m_aspectRatio(aspectRatio),
            m_focalLength(focalLength), m_fov(fov),
            m_viewportHeight(focalLength * std::tan(fov * Math::Deg2RadF * 0.5f) * 2.0f),
            m_viewportWidth(m_viewportHeight * aspectRatio),
            m_viewportU(m_viewportWidth * right),
            m_viewportV(-m_viewportHeight * up),
            m_viewportUpperLeft(m_position + front * focalLength - (m_viewportU + m_viewportV) * 0.5f),
            m_pixel00Location(m_viewportUpperLeft + (m_pixelDeltaU + m_pixelDeltaV) * 0.5f),
            m_samplesPerPixel(samplesPerPixel),
            m_maxDepth(maxDepth)
    {}

    void Camera::Render(char const* imageFile, const Scene& scene, float& process)
    {
        const unsigned int imageDataSize = m_imageWidth * m_imageHeight * 3;
        std::vector<char> imageData(imageDataSize);
        for (unsigned int i = 0; i < m_imageHeight; ++i)
        {
            for (unsigned int j = 0; j < m_imageWidth; ++j)
            {
                Color pixelColor;
                // 抗锯齿
                for (unsigned int sample = 0; sample < m_samplesPerPixel; ++sample)
                {
                    pixelColor += scene.RayColor(GetRay(i, j), m_maxDepth);
                }
                pixelColor /= static_cast<float>(m_samplesPerPixel);
                pixelColor = pixelColor.LinearToGamma();

                const std::size_t offset = (m_imageWidth * i + j) * 3;
                imageData[offset] = static_cast<char>(pixelColor.R() * 255);
                imageData[offset + 1] = static_cast<char>(pixelColor.G() * 255);
                imageData[offset + 2] = static_cast<char>(pixelColor.B() * 255);

                process = static_cast<float>(offset) / static_cast<float>(imageDataSize);
            }
        }
        stbi_write_bmp(imageFile, static_cast<int>(m_imageWidth), static_cast<int>(m_imageHeight),
            3, imageData.data());
    }

    void Camera::Update(const PathTraceData &data)
    {
        m_imageWidth = data.imageWidth;
        m_imageHeight = data.imageHeight;

        m_position = data.cameraPosition;
        m_front = data.front;
        m_up = data.up;
        m_right = data.right;

        m_aspectRatio = data.aspectRatio;
        m_focalLength = data.focalLength;
        m_fov = data.fov;
        m_viewportHeight = m_focalLength * std::tan(m_fov * Math::Deg2RadF * 0.5f) * 2.0f;
        m_viewportWidth = m_viewportHeight * m_aspectRatio;
        m_viewportU = m_viewportWidth * m_right;
        m_viewportV = -m_viewportHeight * m_up;
        m_pixelDeltaU = m_viewportU / static_cast<float>(m_imageWidth);
        m_pixelDeltaV = m_viewportV / static_cast<float>(m_imageHeight);
        m_viewportUpperLeft = m_position + m_front * m_focalLength - (m_viewportU + m_viewportV) * 0.5f;
        m_pixel00Location = m_viewportUpperLeft + (m_pixelDeltaU + m_pixelDeltaV) * 0.5f;

        m_samplesPerPixel = data.samplesPerPixel;
        m_maxDepth = data.maxDepth;
    }

    Math::Geometry::Ray Camera::GetRay(unsigned int row, unsigned int column)
    {
        Math::Vector3f pixelCenter = m_pixel00Location + static_cast<float>(row) * m_pixelDeltaV +
            static_cast<float>(column) * m_pixelDeltaU;
        float px = Math::g_random.Float(-0.5f, 0.5f);
        float py = Math::g_random.Float(-0.5f, 0.5f);
        // 在(row, column)像素内随机取一个采样点
        Math::Vector3f pixelSample = pixelCenter + px * m_pixelDeltaU + py * m_pixelDeltaV;
        Math::Vector3f rayDirection = pixelSample - m_position;
        return { m_position, rayDirection };
    }
}
