/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "QuadExample.h"
#include "Exception/OpenGLException.h"
#include "Diagnostic/Log.hpp"
#include "Geometry/Primitives.h"
#include "Color/StandardColors.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include <fstream>

namespace TG
{
    QuadExample::QuadExample()
    {
        Geometry::Mesh quad = CreatePrimitive(Geometry::PrimitiveType::Quad);

		glGenBuffers(1, &m_VBO);
    	glGenBuffers(1, &m_EBO);
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		// 把顶点数据传入显存
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * quad.vertices.size(), quad.vertices.data(), GL_STATIC_DRAW);
    	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(std::uint32_t) * quad.indices.size(), quad.indices.data(),
    		GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		std::ifstream vertexFile("Shaders/GLSL/simple.vert");
		if (!vertexFile)
			throw BaseException::Create("Failed to load simple vertex shader source file");

		std::ostringstream vertexBuffer;
		vertexBuffer << vertexFile.rdbuf();
		std::string vertexStr = vertexBuffer.str();
		char const* vertexShaderSource = vertexStr.c_str();
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShader);
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			LogError("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", infoLog);
		}

    	std::ifstream geometryFile("Shaders/GLSL/wireframe.geom");
    	if (!geometryFile)
    		throw BaseException::Create("Failed to load wireframe shader source file");
    	std::ostringstream geometryBuffer;
    	geometryBuffer << geometryFile.rdbuf();
    	std::string geometryStr = geometryBuffer.str();
    	char const* geometryShaderSource = geometryStr.c_str();
    	unsigned int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    	glShaderSource(geometryShader, 1, &geometryShaderSource, nullptr);
    	glCompileShader(geometryShader);
    	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
    	if (!success)
    	{
    		glGetShaderInfoLog(geometryShader, 512, nullptr, infoLog);
    		LogError("ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n{}", infoLog);
    	}

		std::ifstream fragmentFile("Shaders/GLSL/simple.frag");
		if (!fragmentFile)
			throw BaseException::Create("Failed to load simple fragment shader source file");

		std::ostringstream fragmentBuffer;
		fragmentBuffer << fragmentFile.rdbuf();
		std::string fragStr = fragmentBuffer.str();
		char const* fragmentShaderSource = fragStr.c_str();
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
			LogError("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n {}", infoLog);
		}

		m_shaderProgram = glCreateProgram();
		glAttachShader(m_shaderProgram, vertexShader);
		glAttachShader(m_shaderProgram, fragmentShader);
		glLinkProgram(m_shaderProgram);
		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
			LogError("ERROR::SHADER::PROGRAM::LINKING_FAILED\n {}", infoLog);
		}

    	m_shaderProgramWireframe = glCreateProgram();
    	glAttachShader(m_shaderProgramWireframe, vertexShader);
   		glAttachShader(m_shaderProgramWireframe, geometryShader);
    	glAttachShader(m_shaderProgramWireframe, fragmentShader);
    	glLinkProgram(m_shaderProgramWireframe);
    	glGetProgramiv(m_shaderProgramWireframe, GL_LINK_STATUS, &success);
    	if (!success) {
    		glGetProgramInfoLog(m_shaderProgramWireframe, 512, nullptr, infoLog);
    		LogError("ERROR::SHADER::PROGRAM::LINKING_FAILED\n {}", infoLog);
    	}

		glDeleteShader(vertexShader);
    	glDeleteShader(geometryShader);
		glDeleteShader(fragmentShader);
    }

    QuadExample::~QuadExample()
    {
    	glDeleteVertexArrays(1, &m_VAO);
    	glDeleteBuffers(1, &m_VBO);
    	glDeleteBuffers(1, &m_EBO);
    	glDeleteProgram(m_shaderProgram);
    }

    void QuadExample::Render()
    {
    	Color::Color clearColor = Color::AliceBlue;

    	glClearColor(clearColor.R(), clearColor.G(), clearColor.B(), clearColor.A());
    	glClear(GL_COLOR_BUFFER_BIT);

    	if (m_wireframe)
    		glUseProgram(m_shaderProgramWireframe);
    	else
	    	glUseProgram(m_shaderProgram);
    	glBindVertexArray(m_VAO);
    	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    	glBindVertexArray(0);

    	ImGui_ImplOpenGL3_NewFrame();
    	ImGui_ImplWin32_NewFrame();
    	ImGui::NewFrame();

		ImGui::Begin("Settings");
		ImGui::Checkbox("Draw wireframe", &m_wireframe);
    	ImGui::End();

    	ImGui::Render();

    	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    	ImGui::UpdatePlatformWindows();
    	ImGui::RenderPlatformWindowsDefault();
    }
}
