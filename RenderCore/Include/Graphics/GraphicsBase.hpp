/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Editor/Window.h"
#include "../Math/Color.hpp"

namespace TG::Graphics
{
	enum class DeviceType: std::uint8_t
	{
		DirectX11,
		DirectX12,
		OpenGL,
		Vulcan,
	};

	struct AdapterDesc
	{
		std::wstring description;
		std::uint32_t videoMemory;		// 单位MB
	};

	struct ICreateInfo
	{
		std::uint16_t adapterIndex;
		HWND hwnd;
	};

	enum class TextureFormat : std::uint16_t
	{
		UNKNOWN = 0,
		R8G8B8A8_UNORM = 28,
	};

	struct SwapChainDesc
	{
		std::uint32_t width;
		std::uint32_t height;
		TextureFormat format;
		std::uint8_t sampleCount;
		std::uint8_t bufferCount;
	};

	// 图形设备
	template <DeviceType T>
	class Device;

	// 设备上下文
	template <DeviceType T>
	class Context;

	// 帧缓存
	template <DeviceType T>
	class FrameBuffer;

#ifdef _DEBUG
	template<DeviceType T>
	class DebugInfo;
#endif
}