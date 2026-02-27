/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "TriangleExample.h"
#include "Rendering/Color/StandardColors.h"
#include "Exception/BaseException.h"
#include "imgui.h"
#include <format>

namespace TG
{
    TriangleExample::TriangleExample()
    {
    	float vertices[] = {
    		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    	};

    	uint32_t indices[] = {
    		0, 1, 2,
    	};

    	glGenBuffers(1, &m_VBO);
    	glGenBuffers(1, &m_EBO);
    	glGenVertexArrays(1, &m_VAO);

    	glBindVertexArray(m_VAO);
    	// 顶点数据传入显存
    	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    	// 设置顶点属性
    	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    	glEnableVertexAttribArray(0);
    	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
			reinterpret_cast<void*>(3 * sizeof(float)));
    	glEnableVertexAttribArray(1);
    	// 顶点索引传入显存
    	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    	glBindVertexArray(0);
    	glBindBuffer(GL_ARRAY_BUFFER, 0);
    	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    	// 创建顶点着色器
    	unsigned int shader = glCreateShader(GL_VERTEX_SHADER);
    	glShaderSource(shader, 1, &m_vertexShaderSource, nullptr);
    	glCompileShader(shader);
    	int success;
    	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    	if (!success)
    	{
    		char infoLog[512];
    		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    		throw BaseException::Create(std::format("Shader compilation failed\n{}", infoLog));
    	}

    	m_vertexShader = glCreateProgram();
    	glAttachShader(m_vertexShader, shader);
    	glProgramParameteri(m_vertexShader, GL_PROGRAM_SEPARABLE, GL_TRUE);
    	glLinkProgram(m_vertexShader);
    	glGetProgramiv(m_vertexShader, GL_LINK_STATUS, &success);
    	if (!success)
    	{
    		char infoLog[512];
    		glGetProgramInfoLog(m_vertexShader, 512, nullptr, infoLog);
    		throw BaseException::Create(std::format("Shader program link failed\n {}", infoLog));
    	}
    	glDeleteShader(shader);

    	// 创建像素着色器
    	shader = glCreateShader(GL_FRAGMENT_SHADER);
    	glShaderSource(shader, 1, &m_fragmentShaderSource, nullptr);
    	glCompileShader(shader);
    	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    	if (!success)
    	{
    		char infoLog[512];
    		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    		throw BaseException::Create(std::format("Shader compilation failed\n{}", infoLog));
    	}

    	m_fragmentShader = glCreateProgram();
    	glAttachShader(m_fragmentShader, shader);
    	glProgramParameteri(m_fragmentShader, GL_PROGRAM_SEPARABLE, GL_TRUE);
    	glLinkProgram(m_fragmentShader);
    	glGetProgramiv(m_fragmentShader, GL_LINK_STATUS, &success);
    	if (!success)
    	{
    		char infoLog[512];
    		glGetProgramInfoLog(m_fragmentShader, 512, nullptr, infoLog);
    		throw BaseException::Create(std::format("Shader program link failed\n {}", infoLog));
    	}
    	glDeleteShader(shader);

    	glGenProgramPipelines(1, &m_pipeline);
    	glBindProgramPipeline(m_pipeline);
    	glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, m_vertexShader);
    	glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, m_fragmentShader);
    	glBindProgramPipeline(0);

    	glDisable(GL_DEPTH_TEST);
    	if (m_wireframe)
    		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    TriangleExample::~TriangleExample()
    {
    	glDeleteProgramPipelines(1, &m_pipeline);
        glDeleteProgram(m_vertexShader);
        glDeleteProgram(m_fragmentShader);
    	glDeleteVertexArrays(1, &m_VAO);
    	glDeleteBuffers(1, &m_VBO);
    	glDeleteBuffers(1, &m_EBO);
    }

    void TriangleExample::Draw()
    {
    	Rendering::Color clearColor = Rendering::Black;

    	glClearColor(clearColor.R(), clearColor.G(), clearColor.B(), clearColor.A());
    	glClearDepthf(1.0f);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	glBindVertexArray(m_VAO);
    	glBindProgramPipeline(m_pipeline);
    	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    	// glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void TriangleExample::DrawUI()
    {
    	const ImGuiIO& io = ImGui::GetIO();
    	ImGui::Begin("Triangle Example Settings");
    	if (ImGui::Checkbox("Draw wireframe", &m_wireframe))
    	{
    		if (m_wireframe)
    			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    		else
    			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    	}
    	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    	ImGui::End();
    }
}
