/****************************************************************
* TianGong Graphics Tool										*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <tuple>
#include <type_traits>
#include <string_view>
#include "ConstString.hpp"
#include "TypeList.hpp"
#include "NameValuePair.hpp"
#include "ElementList.hpp"
#include "AttributeList.hpp"
#include "FieldList.hpp"
#include "Attribute.hpp"
#include "Field.hpp"
#include "Specifier.hpp"
#include "Property.hpp"
#include "Method.hpp"

namespace TG::Reflection
{
    template<typename T> struct TypeInfo;
}
