/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Example.h"
#include "Rendering/Renderer.h"
#include "Rendering/Mesh/Sphere.h"
#include "Rendering/Mesh/Plane.h"
#include "Rendering/RayTracing/PathTracer.h"
#include "Rendering/Gizmo/ViewportGrid.h"
#include "Rendering/Gizmo/ViewportCompass.h"

namespace tinygltf
{
    class Model;
    class Node;
    class Mesh;
}

namespace TG
{
    class BasicLightExample final : public Example
    {
    public:
        BasicLightExample(const Rendering::Renderer* renderer, const ITimer& timer);
        ~BasicLightExample() override;

        void Draw() override;
        void DrawUI() override;

        void OnViewportChanged(unsigned width, unsigned height) override;

    private:
        void LoadRubberToy();

        struct ObjectProperty
        {
            Math::Vector3f position;
            Rendering::Color color;
            float ambientStrength;
            float specularStrength;
            float shininess;
        };

        const ITimer& m_timer;
        Camera m_camera;
        const char* m_cameraType[2]{ "Perspective", "Orthographic" };
        int m_currentType = 0;

        Rendering::Mesh m_rubberToyMesh;
        ObjectProperty m_rubberToyProperty;

        Rendering::Sphere m_sphereMesh;
        ObjectProperty m_sphereProperties[5];

        Rendering::Plane m_planeMesh;
        ObjectProperty m_planeProperty;

        Math::Vector4f m_lightDirection;
        Rendering::Color m_lightColor;

        GLuint m_rubberToyVAO{};
        GLuint m_rubberToyVBO{};
        GLuint m_rubberToyEBO{};
        Rendering::OpenGLTexture2D m_rubberToyAlbedo{};

        GLuint m_sphereVAO{};
        GLuint m_sphereVBO{};
        GLuint m_sphereEBO{};

        GLuint m_planeVAO{};
        GLuint m_planeVBO{};
        GLuint m_planeEBO{};

        GLuint m_pipeline{};
        GLuint m_cameraUbo{};
        GLuint m_renderUbo{};

        Rendering::OpenGLTexture2D m_albedoTexture;
        const unsigned char m_defaultAlbedo[4] = { 255, 255, 255, 255 };

        Rendering::OpenGLShader m_vertexShader;
        Rendering::OpenGLShader m_fragmentShader;
        Rendering::OpenGLShader m_wireframeGeometryShader;
        Rendering::OpenGLShader m_wireframeFragmentShader;

        bool m_wireframe{ false };
        Rendering::ViewportGrid m_viewportGrid;
        Rendering::ViewportCompass m_viewportCompass;

        std::unique_ptr<std::thread> m_renderThread;
        Rendering::RayTracing::PathTracer m_pathTracer;
        float m_focal = 10.0f;
        float m_defocusAngle = 0.6f;
        int m_samplesPerPixel = 50;
        int m_maxDepth = 50;
    };
}
