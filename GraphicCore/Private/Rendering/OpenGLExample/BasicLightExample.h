/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Example.h"
#include "Rendering/Mesh/Sphere.h"
#include "Rendering/Mesh/Plane.h"
#include "Rendering/RayTracing/PathTracer.h"
#include "ViewportGrid.h"
#include "ViewportCompass.h"

namespace TG::Rendering
{
    class BasicLightExample final : public Example
    {
    public:
        BasicLightExample(const std::weak_ptr<IDefaultVideoPort>& videoPort, const std::weak_ptr<ITimer>& timer);
        ~BasicLightExample() override;

        void Render() override;

    private:
        struct ObjectProperty
        {
            Math::Vector3f position;
            Color color;
            float ambientStrength;
            float specularStrength;
            float shininess;
        };

        Camera m_camera;

        Sphere m_sphereMesh;
        ObjectProperty m_sphereProperties[5];
        Plane m_planeMesh;
        ObjectProperty m_planeProperty;
        Math::Vector4f m_lightDirection;
        Color m_lightColor;

        GLuint m_sphereVAO{};
        GLuint m_sphereVBO{};
        GLuint m_sphereEBO{};

        GLuint m_planeVAO{};
        GLuint m_planeVBO{};
        GLuint m_planeEBO{};

        GLuint m_pipeline{};
        GLuint m_cameraUbo{};
        GLuint m_renderUbo{};

        OpenGLTexture2D m_albedoTexture;
        const unsigned char m_defaultAlbedo[4] = { 255, 255, 255, 255 };

        OpenGLShader m_vertexShader;
        OpenGLShader m_fragmentShader;
        OpenGLShader m_wireframeGeometryShader;
        OpenGLShader m_wireframeFragmentShader;

        bool m_wireframe{ false };
        ViewportGrid m_viewportGrid;
        ViewportCompass m_viewportCompass;

        std::unique_ptr<std::thread> m_renderThread;
        PathTracer m_pathTracer;
    };
}
