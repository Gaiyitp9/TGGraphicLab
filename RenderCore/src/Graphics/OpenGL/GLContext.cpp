/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Graphics/OpenGL/GLContext.hpp"
#include "Diagnostics/Debug.hpp"

namespace TG::Graphics
{
	using PFN_wglCreateContext = HGLRC(WINAPI*)(HDC);
	using PFN_wglDeleteContext = BOOL(WINAPI*)(HGLRC);
	using PFN_wglGetProcAddress = PROC(WINAPI*)(LPCSTR);
	using PFN_wglGetCurrentDC = HDC(WINAPI*)(void);
	using PFN_wglGetCurrentContext = HGLRC(WINAPI*)(void);
	using PFN_wglMakeCurrent = BOOL(WINAPI*)(HDC, HGLRC);

	static HMODULE g_glInstance = LoadLibraryA("opengl32.dll");
	static PFN_wglGetProcAddress wglGetProcAddress = (PFN_wglGetProcAddress)GetProcAddress(g_glInstance, "wglGetProcAddress");
	static PFN_wglCreateContext wglCreateContext = (PFN_wglCreateContext)GetProcAddress(g_glInstance, "wglCreateContext");
	static PFN_wglMakeCurrent wglMakeCurrent = (PFN_wglMakeCurrent)GetProcAddress(g_glInstance, "wglMakeCurrent");

	static GLADapiproc GetGLProcAddress(const char* name)
	{
		GLADapiproc proc = reinterpret_cast<GLADapiproc>(wglGetProcAddress(name));
		if (proc)
			return proc;

		return reinterpret_cast<GLADapiproc>(GetProcAddress(g_glInstance, name));
	}

	GLContext::GLContext(const GLCreateInfo& info)
	{
		HDC hdc = GetDC(info.hwnd);
		HGLRC hglrc;

		PIXELFORMATDESCRIPTOR pfd;
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 8;
		int pxfmt = ChoosePixelFormat(hdc, &pfd);
		SetPixelFormat(hdc, pxfmt, &pfd);

		hglrc = wglCreateContext(hdc);
		wglMakeCurrent(hdc, hglrc);

		gladLoadGL(GetGLProcAddress);
		std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	}
}
