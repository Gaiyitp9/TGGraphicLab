/****************************************************************
* TianGong GraphicLab										*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <string>
#include <type_traits>

namespace TG::Reflection
{
    template<typename T> struct TypeInfo;
    template<typename... Types> struct TypeList;
}

#include "Static/ConstString.hpp"
#include "Static/TypeInfo.hpp"
#include "Static/TypeTraits.hpp"
#include "Static/TypeList.hpp"
