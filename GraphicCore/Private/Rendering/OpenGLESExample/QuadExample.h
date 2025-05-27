/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "../Example.h"
#include "Base/CommonInterfaces.h"
#include "Geometry/Mesh.h"
#include "Shader.h"

namespace TG
{
    class QuadExample final : public Example
    {
    public:
        explicit QuadExample(const std::weak_ptr<IDefaultVideoPort>& videoPort, const std::weak_ptr<ITimer> &timer);
        ~QuadExample() override;

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
        Shader m_vertexShader;
        Shader m_fragmentShader;
        Shader m_geometryShader;
        GLuint m_albedo[2]{};
    };
}
