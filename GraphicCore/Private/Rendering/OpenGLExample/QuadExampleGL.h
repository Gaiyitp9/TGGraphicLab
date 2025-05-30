/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../Example.h"
#include "Base/CommonInterfaces.h"
#include "Geometry/Mesh.h"
#include "ShaderGL.h"

namespace TG
{
    class QuadExampleGL final : public Example
    {
    public:
        explicit QuadExampleGL(const std::weak_ptr<IDefaultVideoPort>& videoPort, const std::weak_ptr<ITimer> &timer);
        ~QuadExampleGL() override;

        void Render() override;

    private:
        std::weak_ptr<IDefaultVideoPort> m_videoPort;
        std::weak_ptr<ITimer> m_timer;

        Geometry::Mesh m_quadMesh;

        GLuint m_VAO{};
        GLuint m_VBO{};
        GLuint m_EBO{};
        bool m_wireframe{ false };
        GLuint m_pipeline{};
        ShaderGL m_vertexShader;
        ShaderGL m_fragmentShader;
        ShaderGL m_geometryShader;
        GLuint m_albedo[2]{};
    };
}
