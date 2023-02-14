/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Window/Window.hpp"
#include "../Math/Color.hpp"

namespace TG::Graphics
{
	enum class DeviceType: unsigned char
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
		unsigned int adapterIndex;
		HWND hwnd;
	};

	struct SwapChainDesc
	{

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

	// 缓存格式
	enum class Format : unsigned char
	{
		R8G8B8A8_UNORM = 28,
	};

#ifdef _DEBUG
	template<DeviceType T>
	class DebugInfo;
#endif
}