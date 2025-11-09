/****************************************************************
* TianGong GraphicLab										*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    template<typename T>
    struct DeclaredBaseTypeList
    {

    };

    template<typename T> requires IsReflectable<T>
    class TypeDescriptor
    {
    public:
        using Type = T;


    };
}
