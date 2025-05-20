/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Example.h"
#include "DynamicGles.h"
#include "Base/CommonInterfaces.h"
#include "Geometry/Mesh.h"
#include "Rendering/Camera.h"
#include "Shader.h"
#include "Texture2D.h"

namespace TG
{
    class CubeExample final : public Example
    {
    public:
        CubeExample(const std::weak_ptr<IDefaultVideoPort>& videoPort, const std::weak_ptr<ITimer> &timer);
        ~CubeExample() override;

        void Render() override;

    private:
        std::weak_ptr<ITimer> m_timer;

        Camera m_camera;

        Geometry::Mesh m_cubeMesh;
        Math::Vector3F m_cubePositions[10];

        GLuint m_VAO{};
        GLuint m_VBO{};
        GLuint m_EBO{};
        bool m_wireframe{ false };
        GLuint m_pipeline{};
        Shader m_vertexShader;
        Shader m_fragmentShader;
        Shader m_geometryShader;
        Texture2D m_textures[2]{};
    };
}
