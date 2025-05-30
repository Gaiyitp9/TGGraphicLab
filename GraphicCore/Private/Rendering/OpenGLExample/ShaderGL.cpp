/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "ShaderGL.h"
#include "Exception/BaseException.h"
#include "Diagnostic/Log.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <fstream>
#include <sstream>
#include <format>

namespace TG
{
    ShaderGL::ShaderGL(std::string_view shaderPath, ShaderStageGL shaderStage)
    {
        GLenum target = 0;
        switch (shaderStage)
        {
            case ShaderStageGL::Vertex:
                target = GL_VERTEX_SHADER;
                break;
            case ShaderStageGL::Fragment:
                target = GL_FRAGMENT_SHADER;
                break;
            case ShaderStageGL::Geometry:
                target = GL_GEOMETRY_SHADER;
                break;
            default:
                throw BaseException::Create("Unknown shader target");
        }

        std::ifstream shaderFile(shaderPath.data());
        if (!shaderFile)
            throw BaseException::Create("Failed to load shader source file");

        std::ostringstream shaderBuffer;
        shaderBuffer << shaderFile.rdbuf();
        std::string shaderStr = shaderBuffer.str();
        char const* shaderSource = shaderStr.c_str();
        unsigned int shader = glCreateShader(target);
        glShaderSource(shader, 1, &shaderSource, nullptr);
        glCompileShader(shader);
        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            throw BaseException::Create(std::format("Shader compilation failed\n{}", infoLog));
        }

        m_shaderID = glCreateProgram();
        glAttachShader(m_shaderID, shader);
        glProgramParameteri(m_shaderID, GL_PROGRAM_SEPARABLE, GL_TRUE);
        glLinkProgram(m_shaderID);
        glGetProgramiv(m_shaderID, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(m_shaderID, 512, nullptr, infoLog);
            throw BaseException::Create(std::format("Shader program link failed\n {}", infoLog));
        }

        glDeleteShader(shader);
    }

    ShaderGL::~ShaderGL()
    {
        glDeleteProgram(m_shaderID);
    }

    void ShaderGL::SetBool(std::string_view name, bool value) const
    {
        glProgramUniform1i(m_shaderID, glGetUniformLocation(m_shaderID, name.data()), value);
    }

    void ShaderGL::SetInt(std::string_view name, int value) const
    {
        glProgramUniform1i(m_shaderID, glGetUniformLocation(m_shaderID, name.data()), value);
    }

    void ShaderGL::SetInt2(std::string_view name, int v0, int v1) const
    {
        glProgramUniform2i(m_shaderID, glGetUniformLocation(m_shaderID, name.data()), v0, v1);
    }

    void ShaderGL::SetFloat(std::string_view name, float value) const
    {
        glProgramUniform1f(m_shaderID, glGetUniformLocation(m_shaderID, name.data()), value);
    }

    void ShaderGL::SetFloat2(std::string_view name, float v0, float v1) const
    {
        glProgramUniform2f(m_shaderID, glGetUniformLocation(m_shaderID, name.data()), v0, v1);
    }

    void ShaderGL::SetFloat3(std::string_view name, float v0, float v1, float v3) const
    {
        glProgramUniform3f(m_shaderID, glGetUniformLocation(m_shaderID, name.data()), v0, v1, v3);
    }

    void ShaderGL::SetFloat4(std::string_view name, float v0, float v1, float v2, float v3) const
    {
        glProgramUniform4f(m_shaderID, glGetUniformLocation(m_shaderID, name.data()), v0, v1, v2, v3);
    }

    void ShaderGL::SetMat4(std::string_view name, const glm::mat4 &value) const
    {
        glProgramUniformMatrix4fv(m_shaderID, glGetUniformLocation(m_shaderID, name.data()), 1,
            GL_FALSE, glm::value_ptr(value));
    }

    void ShaderGL::SetMat4(std::string_view name, const Math::Matrix4F& value) const
    {
        glProgramUniformMatrix4fv(m_shaderID, glGetUniformLocation(m_shaderID, name.data()), 1,
            GL_FALSE, value.Data());
    }
}
