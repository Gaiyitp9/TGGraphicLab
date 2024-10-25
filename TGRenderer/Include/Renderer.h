/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Modules/PlatformModule.h"
#include "Modules/InputModule.h"
#include "Modules/RenderModule.h"
#include "Modules/EditorModule.h"

namespace TG
{
	class Renderer
	{
	public:
		Renderer();
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;
		~Renderer();

		int Run();

	private:
		PlatformModule m_platformModule;
		InputModule m_inputModule;
		RenderModule m_renderModule;
		// EditorModule m_editorModule;
	};
}
