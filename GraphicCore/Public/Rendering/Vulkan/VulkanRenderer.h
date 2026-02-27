/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Renderer.h"
#include "Rendering/Vulkan/VulkanContext.h"

namespace TG::Rendering
{
	class VulkanRenderer : public Renderer
	{
	public:
		explicit VulkanRenderer(const IDefaultVideoPort& videoPort);
		~VulkanRenderer() override;

		char const* Type() override;
		
		[[nodiscard]] const IDefaultVideoPort& VideoPort() const override;

		void PreRender() override;

		void Draw(Mesh const* mesh, Material const* material) override;
		void Present() override;

		void ScreenFrameBufferResizeCallback(unsigned int width, unsigned int height) override;
		void SceneFrameBufferResizeCallback(unsigned int width, unsigned int height) override;
		void RenderToTexture() const override;
		void RenderToScreen() const override;

		Texture const* RenderTarget() override;

    	void SetVSync(bool enable) const override;

	private:
		const IDefaultVideoPort& m_videoPort;

		// VulkanContext m_context;
	};
}
