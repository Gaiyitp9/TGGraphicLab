/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <vector>
#include <string_view>

namespace TG
{
    // 分隔字符串
    std::vector<std::string_view> SplitString(std::string_view str, std::string_view delim);
}
