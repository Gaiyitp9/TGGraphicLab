/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "OpenGLRenderer.h"
#include "glad/wgl.h"
#include "Exception/OpenGLException.h"
#include "Diagnostic/Log.hpp"
#include "Rendering/OpenGLExample/OpenGLExampleFactory.h"

namespace TG::Rendering
{
    OpenGLRenderer::OpenGLRenderer(const std::weak_ptr<IDefaultVideoPort>& videoPort,
    	const std::weak_ptr<ITimer>& timer) : m_context(videoPort)
    {
    	if (videoPort.expired() || timer.expired())
    		throw BaseException::Create("Interfaces are not valid");

    	// 查询OpenGL相关信息
    	auto glVersion = reinterpret_cast<char const*>(glGetString(GL_VERSION));
    	if (glVersion == nullptr)
    		throw OpenGLException::Create("Failed to get OpenGL version");
    	LogInfo("OpenGL version: {}", glVersion);
    	auto glVendor = reinterpret_cast<char const*>(glGetString(GL_VENDOR));
    	if (glVendor == nullptr)
    		throw OpenGLException::Create("Failed to get OpenGL vendor");
    	LogInfo("Company for the OpenGL implementation: {} ", glVendor);
    	auto glRenderer = reinterpret_cast<char const*>(glGetString(GL_RENDERER));
    	if (glRenderer == nullptr)
    		throw OpenGLException::Create("Failed to get OpenGL renderer");
    	LogInfo("Name of the OpenGL renderer: {}", glRenderer);
    	auto glShadingLanguageVersion = reinterpret_cast<char const*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
    	if (glShadingLanguageVersion == nullptr)
    		throw OpenGLException::Create("Failed to get OpenGL shading language version");
    	LogInfo("Shading language version: {}", glShadingLanguageVersion);
    	GLint maxAttribs = 0;
    	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
    	LogInfo("Max vertex attributes: {}", maxAttribs);
    	GLint maxVertexTextureImageUnits = 0;
    	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxVertexTextureImageUnits);
    	LogInfo("Max vertex texture image units: {}", maxVertexTextureImageUnits);
    	GLint maxFragmentTextureImageUnits = 0;
    	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxFragmentTextureImageUnits);
    	LogInfo("Max fragment texture image units: {}", maxFragmentTextureImageUnits);
    	GLint maxGeometryTextureImageUnits = 0;
    	glGetIntegerv(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS, &maxGeometryTextureImageUnits);
    	LogInfo("Max geometry texture image units: {}", maxGeometryTextureImageUnits);
    	GLint maxTessellationControlTextureImageUnits = 0;
    	glGetIntegerv(GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS, &maxTessellationControlTextureImageUnits);
    	LogInfo("Max tessellation control texture image units: {}", maxTessellationControlTextureImageUnits);
    	GLint maxTessellationEvaluationTextureImageUnits = 0;
    	glGetIntegerv(GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS, &maxTessellationEvaluationTextureImageUnits);
    	LogInfo("Max tessellation evaluation texture image units: {}", maxTessellationEvaluationTextureImageUnits);
    	GLint maxComputeTextureImageUnits = 0;
    	glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS, &maxComputeTextureImageUnits);
    	LogInfo("Max compute texture image units: {}", maxComputeTextureImageUnits);
    	GLint maxUniformLocations = 0;
    	glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &maxUniformLocations);
    	LogInfo("Max uniform locations: {}", maxUniformLocations);
    	GLint maxVerticesOut;
    	glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES, &maxVerticesOut);
    	LogInfo("Max geometry output vertices: {}", maxVerticesOut);
    	GLfloat lineWidthRange[2];
    	glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange);
    	LogInfo("Line width range: {} - {}", lineWidthRange[0], lineWidthRange[1]);

    	// 正面朝向设置为顺时针
    	// glFrontFace(GL_CW);

    	m_exampleFactory = std::make_unique<OpenGlExampleFactory>(videoPort, timer);
    	m_example = m_exampleFactory->CreateExample(m_exampleEnum);
    }

    OpenGLRenderer::~OpenGLRenderer()
    {
    	m_example.reset();
    	m_exampleFactory.reset();
    }

	void OpenGLRenderer::Render()
	{
    	m_context.MakeCurrent();
    	m_example->Render();
	}

	void OpenGLRenderer::Present()
	{
    	m_context.SwapBuffers();
	}

	void OpenGLRenderer::FrameBufferResizeCallback(unsigned int width, unsigned int height)
	{
		glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
	}
}
