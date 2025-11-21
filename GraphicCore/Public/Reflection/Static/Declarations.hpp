/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    template<typename T>
    struct TypeInfo;

    template<typename... Types>
    struct TypeList;

    /**
     * \brief 判断类型T是否能反射
     */
    template<typename T>
    concept IsReflectable = !requires
    {
        typename TypeInfo<T>::InvalidMarker;
    };


    template<typename T>
    struct TypeDescriptor;
    template<typename T, std::size_t N>
    struct FieldDescriptor;
    template<typename T, std::size_t N>
    struct FunctionDescriptor;
}
