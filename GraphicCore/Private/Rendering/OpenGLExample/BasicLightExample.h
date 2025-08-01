/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Example.h"
#include "Base/CommonInterfaces.h"
#include "Geometry/Sphere.h"
#include "Rendering/Camera.h"
#include "Rendering/OpenGL/OpenGLShader.h"
#include "ViewportGrid.h"
#include "ViewportCompass.h"

namespace TG::Rendering
{
    class BasicLightExample final : public Example
    {
    public:
        BasicLightExample(const std::weak_ptr<IDefaultVideoPort>& videoPort, const std::weak_ptr<ITimer> &timer);
        ~BasicLightExample() override;

        void Render() override;

    private:
        Camera m_camera;

        Geometry::Sphere m_sphereMesh;
        Math::Vector3F m_spherePositions[10];

        GLuint m_VAO{};
        GLuint m_VBO{};
        GLuint m_EBO{};
        GLuint m_pipeline{};
        GLuint m_cameraUbo{};
        GLuint m_lightUbo{};

        OpenGLShader m_vertexShader;
        OpenGLShader m_fragmentShader;

        ViewportGrid m_viewportGrid;
        ViewportCompass m_viewportCompass;
    };
}
