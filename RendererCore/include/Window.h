#pragma once

#include <Windows.h>

namespace LCH
{
	class Window
	{
	public:
		Window();
		~Window();

	public:
		HWND hwnd;
	};
}