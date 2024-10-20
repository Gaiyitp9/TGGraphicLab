/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Chronometer.h"
// #include "Utility.h"
#include <ctime>
#include <string>
#include "../../GraphicCore/Include/Math/Core.hpp"
#include "../../GraphicCore/Include/Input/Event.h"
// #include "Diagnostics/Win32Exception.h"
// #include "glad/gl.h"

namespace TG
{
	class UnitTest
	{
		// using PFN_wglCreateContext = HGLRC(WINAPI*)(HDC);
		// using PFN_wglDeleteContext = BOOL(WINAPI*)(HGLRC);
		// using PFN_wglGetProcAddress = PROC(WINAPI*)(LPCSTR);
		// using PFN_wglGetCurrentDC = HDC(WINAPI*)(void);
		// using PFN_wglGetCurrentContext = HGLRC(WINAPI*)(void);
		// using PFN_wglMakeCurrent = BOOL(WINAPI*)(HDC, HGLRC);
	public:
		UnitTest();
		~UnitTest();
	public:
		void FormatTest();
		void TextEncodeTest();
		void TimeTest();
		void ArrayAlignmentTest();
		void MathLibTest();
		void SIMDTest();
		// void OpenGLTest(HWND);

	private:
		void NormalAdd(int* nums, size_t n);
		void NormalAddf(float* nums, size_t n);
		void SSEAdd(int* nums, size_t n);
		void SSEAddf(float* nums, size_t n);
		void AVX2Add(int* nums, size_t n);
		void AVX2Addf(float* nums, size_t n);

	public:
//		static HMODULE glInst;
//		static PFN_wglGetProcAddress wglGetProcAddress;
//		PFN_wglCreateContext wglCreateContext = (PFN_wglCreateContext)GetProcAddress(glInst, "wglCreateContext");
//		PFN_wglMakeCurrent wglMakeCurrent = (PFN_wglMakeCurrent)GetProcAddress(glInst, "wglMakeCurrent");
	private:
		Chronometer timer;
	};
}