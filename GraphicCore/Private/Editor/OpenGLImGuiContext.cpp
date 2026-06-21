/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Editor/OpenGLImGuiContext.h"


namespace TG::Editor
{
	OpenGLImGuiContext::OpenGLImGuiContext(const Rendering::OpenGLContext& context)
	{

    }

	OpenGLImGuiContext::~OpenGLImGuiContext() {}

	void OpenGLImGuiContext::NewFrame()
	{
		m_context.NewFrame();
	}

	void OpenGLImGuiContext::Render()
	{
		m_context.Render();
	}
}
