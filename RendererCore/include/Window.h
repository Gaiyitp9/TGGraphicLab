/********************************************************
* Copyright (c) LiaoChenhan. All rights reserved.		*
*														*
* This code is licensed under the MIT License (MIT).	*
* THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF		*
* ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY	*
* IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR		*
* PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.		*
*********************************************************/

#pragma once

#include "pch.h"

namespace LCH
{
	enum class WindowType
	{
		Default
	};

	class WindowClassRegister
	{
	public:
		WindowClassRegister();
	public:
		std::unordered_map<WindowType, std::string> windowClassMap;
	};

	class Window
	{
	public:
		Window(HINSTANCE hInstance, float width, float height, WindowType type = WindowType::Default);
		~Window();

	private:
		// ´°¿ÚÀà×¢²áÆ÷µ¥Àý
		static WindowClassRegister wcRegister;

	public:
		HWND hwnd;
		HINSTANCE hInstance;
	};
}