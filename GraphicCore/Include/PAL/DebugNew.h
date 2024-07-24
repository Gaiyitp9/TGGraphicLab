/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

// 用于Windows平台内存泄漏检测，使用TG_NEW或者malloc来申请内存，不要直接使用new，否则无法获取内存泄漏的详细信息
// 在CMake里定义_CRTDBG_MAP_ALLOC宏并传给编译器
#if defined(_DEBUG) && defined(_WIN64)
#include <crtdbg.h>
#define TG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define TG_NEW new
#endif
