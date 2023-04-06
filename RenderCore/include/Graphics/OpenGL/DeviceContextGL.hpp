/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "GLContextInfo.hpp"

namespace TG::Graphics
{
	class DeviceContextGL
	{
	public:
        DeviceContextGL() = default;
		explicit DeviceContextGL(const GLContextInfo& info);
        ~DeviceContextGL();

        void MakeContextCurrent() const;        // 将设备上下文设置为线程当前的上下文，如果有多个设备上下文，就可以调用这个函数来切换

        HDC m_hdc;
	private:
		HWND m_hwnd;
        HGLRC m_hglrc;
	};
}
