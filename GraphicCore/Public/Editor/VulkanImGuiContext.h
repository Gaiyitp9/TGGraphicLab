/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "ImGuiContext.h"
#include "Rendering/Vulkan/VulkanContext.h"

namespace TG::Editor
{
	class VulkanImGuiContext : public ImGuiContext
	{
	public:
		explicit VulkanImGuiContext(const Rendering::VulkanContext& context);
		~VulkanImGuiContext() override;

		void NewFrame() override;
		void Render() override;

	private:
		const Rendering::VulkanContext& m_context;
	};
}
