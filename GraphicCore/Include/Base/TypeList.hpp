/****************************************************************
* TianGong GraphicLab										*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <type_traits>

namespace TG
{
    template<typename... Types>
    struct TypeList
    {
        constexpr static std::size_t Size = sizeof...(Types);
    };
}
