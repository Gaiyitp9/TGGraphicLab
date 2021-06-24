/********************************************************
* Copyright (c) LiaoChenhan. All rights reserved.		*
*														*
* This code is licensed under the MIT License (MIT).	*
* THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF		*
* ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY	*
* IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR		*
* PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.		*
*********************************************************/

#include "Window.h"

LCH::WindowClassRegister::WindowClassRegister()
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
}

LCH::Window::Window(HINSTANCE hInstance, float width, float height, WindowType type)
{
}
