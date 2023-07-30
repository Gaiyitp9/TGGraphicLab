/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <tuple>
#include <type_traits>
#include <string_view>
#include "NameValuePair.hpp"
#include "FieldList.hpp"
#include "Attribute.hpp"
#include "Field.hpp"

namespace TG::Reflection
{
    template<typename T> struct TypeInfo;
}
