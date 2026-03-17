/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Editor
{
	class ImGuiContext
	{
	public:
		virtual ~ImGuiContext() = default;

		virtual void NewFrame() = 0;
		virtual void Render() = 0;
	};
}
