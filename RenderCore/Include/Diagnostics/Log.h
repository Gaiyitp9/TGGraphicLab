/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <iostream>

namespace TG::Debug
{
    // 注意，不会刷新缓冲区到设备
    inline void Log(std::string_view log) { std::cout << log; }
    inline void LogLine(std::string_view log) { std::cout << log << '\n'; }
}
