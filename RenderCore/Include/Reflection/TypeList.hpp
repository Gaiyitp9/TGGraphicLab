/****************************************************************
* TianGong RenderLab										*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    template<typename... Types>
    class TypeList
    {
        constexpr static std::size_t Size = sizeof...(Types);
    };
}