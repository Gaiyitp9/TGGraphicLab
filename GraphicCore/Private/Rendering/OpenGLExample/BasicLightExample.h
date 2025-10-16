/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Example.h"
#include "Geometry/Sphere.h"
#include "Color/Color.h"
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
            Color::Color color;
            float ambientStrength;
            float specularStrength;
            float shininess;
        };

        Camera m_camera;

        Geometry::Sphere m_sphereMesh;
        ObjectProperty m_sphereProperties[5];
        Math::Vector4f m_lightDirection;
        Color::Color m_lightColor;

        GLuint m_VAO{};
        GLuint m_VBO{};
        GLuint m_EBO{};
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
        std::atomic<float> m_renderProcess{ 0.0f };
        std::atomic_bool m_renderDone{ true };
    };
}
