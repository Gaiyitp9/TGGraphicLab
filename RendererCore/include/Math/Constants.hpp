/********************************************************************
* TianGong RenderLab												*
* Copyright (c) Gaiyitp9. All rights reserved.						*
* This code is licensed under the MIT License (MIT).				*
*																	*
* Noted:															*
* This file is part of Eigen, a lightweight C++ template library	*
* for linear algebra which is subject to the terms of the			*
* Mozilla Public License v.2.0. And I made some simplifications.	*
*********************************************************************/
#pragma once

namespace LCH::Math
{

enum class StorageOption : char
{
	RowMajor = 0x1,
	ColMajor = 0x2,
};
const StorageOption DefaultMatrixStorageOrderOption = StorageOption::ColMajor;
const int Dynamic = -1;

enum class Flag : unsigned int
{
	None				= 0,
	RowMajor			= 0x1,
	EvalBeforeNesting	= 0x2,
	PacketAccess		= 0x4,
	LinearAccess		= 0x10,
	Lvalue				= 0x20,
	DirectAccess		= 0x40,		// 表示可以直接访问底层数组，包含data()函数
	NestByRef			= 0x100,
};

enum class AccessorLevel : char
{
	ReadOnly,
	Write,
	Direct,
	DirectWrite,
};

enum class TraversalType : char
{
	Default,
	Linear,
	InnerVectorized,
	LinearVectorized,
	Invalid,
	AllAtOnce,
};

enum class UnrollingType : char
{
	None,
	Complete,
};

template<typename Enum> requires std::is_enum_v<Enum>
constexpr auto underlying_type_cast(Enum e) { return static_cast<std::underlying_type_t<Enum>>(e); }

template<typename Enum> requires std::is_enum_v<Enum>
constexpr Enum operator|(const Enum& left, const Enum& right)
{
	return static_cast<Enum>(underlying_type_cast(left) | underlying_type_cast(right));
}

template<typename Enum> requires std::is_enum_v<Enum>
constexpr Enum operator^(const Enum& left, const Enum& right)
{
	return static_cast<Enum>(underlying_type_cast(left) ^ underlying_type_cast(right));
}

template<typename Enum> requires std::is_enum_v<Enum>
constexpr Enum operator&(const Enum& left, const Enum& right)
{
	return static_cast<Enum>(underlying_type_cast(left) & underlying_type_cast(right));
}

template<typename Enum> requires std::is_enum_v<Enum>
constexpr Enum operator~(const Enum& e)
{
	return static_cast<Enum>(~underlying_type_cast(e));
}

template<typename Enum> struct enum_traits { constexpr static bool has_none = false; };
template<>struct enum_traits<Flag> { constexpr static bool has_none = true; };

template<typename Enum> requires std::is_enum_v<Enum> && enum_traits<Enum>::has_none
constexpr bool operator!(const Enum& e)
{
	return e == Enum::None;
}

template<typename Enum> requires std::is_enum_v<Enum> && enum_traits<Enum>::has_none
constexpr bool not_none(const Enum& e) { return e != Enum::None; }

}