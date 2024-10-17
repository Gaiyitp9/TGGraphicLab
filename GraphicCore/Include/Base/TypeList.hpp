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
        static constexpr std::size_t Size = sizeof...(Types);
    };
}
