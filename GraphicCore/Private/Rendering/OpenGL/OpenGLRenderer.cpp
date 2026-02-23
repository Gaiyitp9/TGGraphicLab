/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Rendering/OpenGL/OpenGLRenderer.h"
#include "Exception/OpenGLException.h"
#include "Diagnostic/Log.hpp"
#include "Rendering/Texture.h"

namespace TG::Rendering
{
    OpenGLRenderer::OpenGLRenderer(const IDefaultVideoPort& videoPort)
		: m_context(videoPort)
    {
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
    	GLint maxUniformBufferBindings;
    	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxUniformBufferBindings);
    	LogInfo("Max uniform buffer bindings: {}", maxUniformBufferBindings);
    	GLint64 maxUniformBlockSize;
    	glGetInteger64v(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
    	LogInfo("Max uniform block size: {}", maxUniformBlockSize);

    	// 创建离屏帧缓存
    	glGenFramebuffers(1, &m_fbo);
    	// 创建深度模板缓存
    	glGenRenderbuffers(1, &m_depthStencilBuffer);

    	GLint viewport[4];
    	glGetIntegerv(GL_VIEWPORT, viewport);
    	m_screenWidth = viewport[2] - viewport[0];
    	m_screenHeight = viewport[3] - viewport[1];
    }

    OpenGLRenderer::~OpenGLRenderer()
    {
		glDeleteFramebuffers(1, &m_fbo);
    	glDeleteRenderbuffers(1, &m_depthStencilBuffer);
    }

	void OpenGLRenderer::PreRender()
	{
    	m_context.MakeCurrent();

    	RenderToTexture();
	}

	void OpenGLRenderer::Draw(Mesh const* mesh, Material const* material)
	{

	}

	void OpenGLRenderer::Present()
	{
    	m_context.SwapBuffers();
	}

	void OpenGLRenderer::ScreenFrameBufferResizeCallback(unsigned int width, unsigned int height)
	{
    	m_screenWidth = width;
    	m_screenHeight = height;
	}

	void OpenGLRenderer::SceneFrameBufferResizeCallback(unsigned int width, unsigned int height)
	{
		m_sceneWidth = width;
    	m_sceneHeight = height;
    	ResizeFrameBuffer(width, height);
	}

	void OpenGLRenderer::RenderToTexture() const
	{
	    glBindFramebuffer(GL_FRAMEBUFFER, CastID<OpenGLID>(m_framebufferTexture.GetID()));
		glViewport(0, 0, static_cast<GLsizei>(m_sceneWidth), static_cast<GLsizei>(m_sceneHeight));
    }

	void OpenGLRenderer::RenderToScreen() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, static_cast<GLsizei>(m_screenWidth), static_cast<GLsizei>(m_screenHeight));
	}

	Texture const* OpenGLRenderer::RenderTarget()
	{
		return &m_framebufferTexture;
	}

	void OpenGLRenderer::ResizeFrameBuffer(unsigned int width, unsigned int height)
	{
    	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    	// 颜色附件
    	m_framebufferTexture.Resize(static_cast<int>(width), static_cast<int>(height));
    	m_framebufferTexture.FilteringMode(TextureFilteringMode::BILINEAR);
    	m_framebufferTexture.WrapMode(TextureWrapMode::CLAMP_TO_EDGE);
    	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
    		CastID<OpenGLID>(m_framebufferTexture.GetID()), 0);

    	// 深度和模板附件
    	glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);
    	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
    		static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
    		GL_RENDERBUFFER, m_depthStencilBuffer);
    	glBindRenderbuffer(GL_RENDERBUFFER, 0);

    	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    		throw BaseException::Create("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
