/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Rendering/Mesh/MeshRendererComponent.h"

namespace TG::Rendering
{
    MeshRendererComponent::MeshRendererComponent(Object* owner) : Component(owner)
    {
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
        glGenVertexArrays(1, &m_VAO);
    }

    void MeshRendererComponent::Update(float deltaTime)
    {

    }

    std::string MeshRendererComponent::GetName() const
    {
        return "MeshRendererComponent";
    }

    void MeshRendererComponent::SetMesh(std::shared_ptr<Mesh> mesh)
    {
        m_mesh = mesh;

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        const auto verticesByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_mesh->vertices.size());
        const auto uvByteSize = static_cast<GLsizeiptr>(2 * sizeof(float) * m_mesh->uvs.size());
        glBufferData(GL_ARRAY_BUFFER, verticesByteSize + uvByteSize, nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, verticesByteSize, m_mesh->vertices.data());
        glBufferSubData(GL_ARRAY_BUFFER, verticesByteSize, uvByteSize, m_mesh->uvs.data());

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void*>(verticesByteSize));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(sizeof(std::uint32_t) * m_mesh->indices.size()),
            m_mesh->indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void MeshRendererComponent::SetMaterial()
    {

    }
}
