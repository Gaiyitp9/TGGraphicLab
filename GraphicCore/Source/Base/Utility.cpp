/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Utility.h"

namespace TG
{
    std::vector<std::string_view> SplitString(std::string_view str, std::string_view delim)
    {
        std::vector<std::string_view> tokens;
        std::size_t start = 0, end = 0;
        while ((end = str.find(delim, start)) != std::string_view::npos)
        {
            tokens.push_back(str.substr(start, end - start));
            start = end + delim.size();
        }
        tokens.push_back(str.substr(start));
        return tokens;
    }
}
