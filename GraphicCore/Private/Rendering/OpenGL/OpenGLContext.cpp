/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Rendering/OpenGL/OpenGLContext.h"
#include "Rendering/OpenGL/CreateContext.hpp"

namespace TG::Rendering
{
    OpenGLContext::OpenGLContext(const IVideoPort& videoPort)
	    : m_videoPort{videoPort},
		m_context{CreateContext<g_platform>(videoPort)}
    {}

    OpenGLContext::~OpenGLContext() = default;

	const IVideoPort& OpenGLContext::VideoPort() const
    {
	    return m_videoPort;
    }

	void OpenGLContext::MakeCurrent() const
	{
    	m_context->MakeCurrent();
	}

    void OpenGLContext::SetVSync(bool enable) const
    {
        m_context->SetVSync(enable);
    }

    void OpenGLContext::Present() const
    {
		m_context->Present();
    }
}
