/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#ifdef TG_SHARED_LIB
    #ifdef TG_WINDOWS
        #ifdef GraphicCore_EXPORTS
            #define TG_API __declspec(dllexport)
        #else
            #define TG_API _declspec(dllimport)
        #endif
    #else
        #define TG_API __attribute__((visibility("default")))
    #endif
#else
    #define TG_API
#endif
