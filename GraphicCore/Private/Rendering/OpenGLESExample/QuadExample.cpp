/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "QuadExample.h"
#include "Color/StandardColors.h"
#include "Exception/BaseException.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "stb_image.h"

namespace TG
{
    QuadExample::QuadExample(const ITimer& timer) : m_timer(timer),
		m_vertexShader("Shaders/GLSL/simple.vert", ShaderStage::Vertex),
		m_fragmentShader("Shaders/GLSL/simple.frag", ShaderStage::Fragment),
		m_geometryShader("Shaders/GLSL/wireframe.geom", ShaderStage::Geometry)
    {
	    float vertices[] = {
	    	// positions			colors			  uvs
	    	-0.5f, -0.5f, 0.0f, 1.0f, 0.5f, 0.2f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
		};
    	std::uint32_t indices[] = {
    		0, 1, 2,
			2, 3, 0,
		};

    	glGenBuffers(1, &m_VBO);
    	glGenBuffers(1, &m_EBO);
    	glGenVertexArrays(1, &m_VAO);
    	glBindVertexArray(m_VAO);
    	// 把顶点数据传入显存
    	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    	// 设置顶点属性
    	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    	glEnableVertexAttribArray(0);
    	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			reinterpret_cast<void*>(3 * sizeof(float)));
    	glEnableVertexAttribArray(1);
    	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			reinterpret_cast<void*>(6 * sizeof(float)));
    	glEnableVertexAttribArray(2);
    	glBindVertexArray(0);
    	// 加载贴图
    	glGenTextures(2, m_albedo);
    	glBindTexture(GL_TEXTURE_2D, m_albedo[0]);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    	int width, height, nrChannels;
    	unsigned char* data = stbi_load("Resources/Textures/wall.jpg", &width, &height, &nrChannels, 0);
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
		data = stbi_load("Resources/Textures/container.jpg", &width, &height, &nrChannels, 0);
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

    	m_fragmentShader.Activate();
    	m_fragmentShader.SetInt("albedo0", 0);
    	m_fragmentShader.SetInt("albedo1", 1);
    	m_fragmentShader.Deactivate();

    	glGenProgramPipelines(1, &m_pipeline);
    	glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, m_vertexShader.GetId());
    	glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, m_fragmentShader.GetId());
    }

    QuadExample::~QuadExample()
    {
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
    		glUseProgramStages(m_pipeline, GL_GEOMETRY_SHADER_BIT, m_geometryShader.GetId());
    	else
    		glUseProgramStages(m_pipeline, GL_GEOMETRY_SHADER_BIT, 0);

    	float timeValue = m_timer.GetTime() * 0.001f;
    	float greyValue = std::sin(timeValue) * 0.5f + 0.5f;
    	m_fragmentShader.Activate();
    	m_fragmentShader.SetFloat4("ourColor", greyValue, greyValue, greyValue, 1.0f);
    	m_fragmentShader.Deactivate();

    	glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, m_albedo[0]);
    	glActiveTexture(GL_TEXTURE1);
    	glBindTexture(GL_TEXTURE_2D, m_albedo[1]);

    	glBindVertexArray(m_VAO);
    	glBindProgramPipeline(m_pipeline);
    	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    	glBindVertexArray(0);

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
