/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Component.h"
#include "Rendering/Mesh/Mesh.h"
#include "Rendering/Shader.h"
#include <memory>

namespace TG::Rendering
{
    class MeshRendererComponent final : public Component
    {
    public:
        MeshRendererComponent(Object* owner);

        void Update(float deltaTime) override;
        [[nodiscard]] std::string GetName() const override;

        void SetMesh(std::shared_ptr<Mesh> mesh);
        void SetMaterial();

    private:
        std::shared_ptr<Mesh> m_mesh;
        GLuint m_VAO{};
        GLuint m_VBO{};
        GLuint m_EBO{};
        std::shared_ptr<Shader> m_Shader;
    };
}
