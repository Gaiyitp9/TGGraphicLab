/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "QuadExample.h"
#include "Color/StandardColors.h"
#include "Exception/BaseException.h"
#include "Geometry/Primitives.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "stb_image.h"

namespace TG::Rendering
{
    QuadExample::QuadExample(const std::weak_ptr<IDefaultVideoPort>& videoPort, const std::weak_ptr<ITimer> &timer)
		: m_videoPort(videoPort), m_timer(timer),
		m_vertexShader("Assets/Shaders/GLSL/Quad.vert", ShaderStage::Vertex),
		m_fragmentShader("Assets/Shaders/GLSL/Quad.frag", ShaderStage::Fragment),
		m_wireframeGeometryShader("Assets/Shaders/GLSL/Wireframe.geom", ShaderStage::Geometry),
		m_wireframeFragmentShader("Assets/Shaders/GLSL/Wireframe.frag", ShaderStage::Fragment)
    {
    	m_quadMesh = Geometry::CreatePrimitive(Geometry::PrimitiveType::Quad);
		m_quadMesh.colors[0] = { 1.0f, 0.5f, 0.2f };
		m_quadMesh.colors[1] = { 1.0f, 0.0f, 0.0f };
		m_quadMesh.colors[2] = { 0.0f, 1.0f, 0.0f };
		m_quadMesh.colors[3] = { 0.0f, 0.0f, 1.0f };

    	glGenBuffers(1, &m_VBO);
    	glGenBuffers(1, &m_EBO);
    	glGenVertexArrays(1, &m_VAO);

    	glBindVertexArray(m_VAO);
    	// 顶点数据传入显存
    	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    	const auto positionByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_quadMesh.vertices.size());
    	const auto uvByteSize = static_cast<GLsizeiptr>(2 * sizeof(float) * m_quadMesh.uvs.size());
    	const auto colorByteSize = static_cast<GLsizeiptr>(3 * sizeof(float) * m_quadMesh.colors.size());
    	glBufferData(GL_ARRAY_BUFFER, positionByteSize + uvByteSize + colorByteSize, nullptr, GL_STATIC_DRAW);
    	glBufferSubData(GL_ARRAY_BUFFER, 0, positionByteSize, m_quadMesh.vertices.data());
    	glBufferSubData(GL_ARRAY_BUFFER, positionByteSize, uvByteSize, m_quadMesh.uvs.data());
    	glBufferSubData(GL_ARRAY_BUFFER, positionByteSize + uvByteSize, colorByteSize, m_quadMesh.colors.data());
    	// 设置顶点属性
    	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    	glEnableVertexAttribArray(0);
    	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
			reinterpret_cast<void*>(positionByteSize));
    	glEnableVertexAttribArray(1);
    	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			reinterpret_cast<void*>(positionByteSize + uvByteSize));
    	glEnableVertexAttribArray(2);
    	// 顶点索引传入显存
    	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
    		static_cast<GLsizeiptr>(sizeof(std::uint32_t) * m_quadMesh.indices.size()),
    		m_quadMesh.indices.data(), GL_STATIC_DRAW);
    	glBindVertexArray(0);

    	// 加载贴图
    	glGenTextures(2, m_albedo);
    	glBindTexture(GL_TEXTURE_2D, m_albedo[0]);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    	int width, height, nrChannels;
    	unsigned char* data = stbi_load("Assets/Textures/wall.jpg", &width, &height, &nrChannels, 0);
    	if (data)
    	{
    		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    		glGenerateMipmap(GL_TEXTURE_2D);
    	}
    	else
    	{
    		throw BaseException::Create("Failed to load texture");
    	}
    	stbi_image_free(data);

    	glBindTexture(GL_TEXTURE_2D, m_albedo[1]);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		data = stbi_load("Assets/Textures/container.jpg", &width, &height, &nrChannels, 0);
    	if (data)
    	{
    		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    		glGenerateMipmap(GL_TEXTURE_2D);
    	}
    	else
    	{
    		throw BaseException::Create("Failed to load texture");
    	}
    	stbi_image_free(data);

    	m_fragmentShader.SetInt("albedo0", 0);
    	m_fragmentShader.SetInt("albedo1", 1);

    	glGenProgramPipelines(1, &m_pipeline);
    	glBindProgramPipeline(m_pipeline);
    	glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, CastID<GLuint>(m_vertexShader.GetID()));
    }

    QuadExample::~QuadExample()
    {
    	glDeleteTextures(2, m_albedo);
    	glDeleteVertexArrays(1, &m_VAO);
    	glDeleteBuffers(1, &m_VBO);
    	glDeleteBuffers(1, &m_EBO);
    	glDeleteProgramPipelines(1, &m_pipeline);
    }

    void QuadExample::Render()
    {
    	Color::Color clearColor = Color::AliceBlue;

    	glClearColor(clearColor.R(), clearColor.G(), clearColor.B(), clearColor.A());
    	glClear(GL_COLOR_BUFFER_BIT);

    	if (m_wireframe)
    	{
    		glUseProgramStages(m_pipeline, GL_GEOMETRY_SHADER_BIT, CastID<GLuint>(m_wireframeGeometryShader.GetID()));
    		glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, CastID<GLuint>(m_wireframeFragmentShader.GetID()));
    	}
    	else
    	{
    		glUseProgramStages(m_pipeline, GL_GEOMETRY_SHADER_BIT, 0);
    		glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, CastID<GLuint>(m_fragmentShader.GetID()));
    	}

    	if (auto timerPtr = m_timer.lock())
    	{
    		const float timeValue = timerPtr->TotalTime() * 0.001f;
    		const float greyValue = std::sin(timeValue) * 0.5f + 0.5f;
    		m_fragmentShader.SetFloat4("ourColor", greyValue, greyValue, greyValue, 1.0f);

    		Math::Transform<float, 3> transform;
    		transform.Translate(Math::Vector3F{ 0.5f, -0.5f, 0.0f });
    		transform.Rotate(Math::AngleAxis{ timeValue, Math::Vector3F{ 0.0f, 0.0f, 1.0f }});
    		m_vertexShader.SetMat4("transform", transform.ToTransformMatrix());
    	}

    	glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, m_albedo[0]);
    	glActiveTexture(GL_TEXTURE1);
    	glBindTexture(GL_TEXTURE_2D, m_albedo[1]);

    	glBindVertexArray(m_VAO);
    	glBindProgramPipeline(m_pipeline);
    	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_quadMesh.indices.size()), GL_UNSIGNED_INT, nullptr);

    	ImGui_ImplOpenGL3_NewFrame();
    	ImGui_ImplWin32_NewFrame();
    	ImGui::NewFrame();

    	const ImGuiIO& io = ImGui::GetIO();
		ImGui::Begin("Settings");
		ImGui::Checkbox("Draw wireframe", &m_wireframe);
    	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    	ImGui::End();

    	ImGui::Render();

    	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    	ImGui::UpdatePlatformWindows();
    	ImGui::RenderPlatformWindowsDefault();
    }
}
