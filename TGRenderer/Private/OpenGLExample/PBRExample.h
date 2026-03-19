/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Example.h"
#include "Rendering/Renderer.h"
#include "Rendering/Color/Color.h"
#include "Rendering/Mesh/Sphere.h"
#include "Rendering/OpenGL/OpenGLTexture2D.h"
#include "Rendering/OpenGL/SkyBox.h"
#include "Rendering/Gizmo/ViewportCompass.h"

namespace TG
{
    class PBRExample : public Example
    {
    public:
        PBRExample(const Rendering::Renderer* renderer, const ITimer& timer);
        ~PBRExample() override;

        void Draw() override;
        void DrawUI() override;

        void OnViewportChanged(unsigned width, unsigned height) override;

    private:
        struct ObjectProperty
        {
            Math::Vector3f position;
        };

        Camera m_camera;

        Math::Vector4f m_lightDirection;
        Rendering::Color m_lightColor;

        Rendering::Sphere m_sphereMesh;
        ObjectProperty m_sphereProperties;

        GLuint m_sphereVAO{};
        GLuint m_sphereVBO{};
        GLuint m_sphereEBO{};

        Rendering::OpenGLShader m_vertexShader;
        Rendering::OpenGLShader m_fragmentShader;

        Rendering::OpenGLTexture2D m_albedo;
        Rendering::OpenGLTexture2D m_normalMap;
        Rendering::OpenGLTexture2D m_metallicRoughness;

        GLuint m_pipeline{};
        GLuint m_cameraUbo{};
        GLuint m_renderUbo{};

        Rendering::SkyBox m_skybox;
        Rendering::ViewportCompass m_viewportCompass;
    };
}
