/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "ImGuiContext.h"
#include "Rendering/OpenGL/OpenGLContext.h"

namespace TG::Editor
{
	class OpenGLImGuiContext : public ImGuiContext
	{
	public:
		explicit OpenGLImGuiContext(const Rendering::OpenGLContext& context);
		~OpenGLImGuiContext() override;

		void NewFrame() override;
		void Render() override;
	};
}
