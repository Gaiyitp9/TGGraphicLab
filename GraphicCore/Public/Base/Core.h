/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#ifdef TG_WINDOWS
    // 用于Windows平台内存泄漏检测，使用TG_NEW或者malloc来申请内存，不要直接使用new，否则无法获取内存泄漏的详细信息
    // 在CMake里定义_CRTDBG_MAP_ALLOC宏并传给编译器
    #ifdef _DEBUG
        #include <crtdbg.h>
        #define TG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
    #else
        #define TG_NEW new
    #endif

    #define TG_API
    #ifdef TG_SHARED_LIB
        #ifdef GraphicCore_EXPORTS
            #define TG_API __declspec(dllexport)
        #else
            #define TG_API _declspec(dllimport)
        #endif
    #endif
#endif

#ifdef TG_LINUX
    #define TG_API
    #ifdef TG_SHARED_LIB
        #define TG_API __attribute__((visibility("default")))
    #endif
#endif
