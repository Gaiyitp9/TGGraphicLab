/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once
#include <PlatformHeaders.h>
#include <glad/gl.h>

namespace TG::Graphics
{
	class GLContext
	{
		using PFN_wglCreateContext = HGLRC(WINAPI*)(HDC);
		using PFN_wglDeleteContext = BOOL(WINAPI*)(HGLRC);
		using PFN_wglGetProcAddress = PROC(WINAPI*)(LPCSTR);
		using PFN_wglGetCurrentDC = HDC(WINAPI*)(void);
		using PFN_wglGetCurrentContext = HGLRC(WINAPI*)(void);
		using PFN_wglMakeCurrent = BOOL(WINAPI*)(HDC, HGLRC);
		
	private:
		static PFN_wglGetProcAddress wglGetProcAddress;
		//PFN_wglCreateContext wglCreateContext = (PFN_wglCreateContext)GetProcAddress(glInst, "wglCreateContext");
		//PFN_wglMakeCurrent wglMakeCurrent = (PFN_wglMakeCurrent)GetProcAddress(glInst, "wglMakeCurrent");
	};
}
