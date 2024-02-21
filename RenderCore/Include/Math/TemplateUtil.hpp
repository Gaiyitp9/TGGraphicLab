/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <type_traits>

namespace TG::Math
{
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

	template<typename Enum> requires std::is_enum_v<Enum>&& enum_traits<Enum>::has_none
		constexpr bool operator!(const Enum& e)
	{
		return e == Enum::None;
	}

	template<typename Enum> requires std::is_enum_v<Enum>&& enum_traits<Enum>::has_none
		constexpr bool NotNone(const Enum& e) { return e != Enum::None; }

	template<typename T> struct remove_all { using type = T; };
	template<typename T> struct remove_all<const T> { using type = remove_all<T>::type; };
	template<typename T> struct remove_all<const T&> { using type = remove_all<T>::type; };
	template<typename T> struct remove_all<T&> { using type = remove_all<T>::type; };
	template<typename T> struct remove_all<T const*> { using type = remove_all<T>::type; };
	template<typename T> struct remove_all<T*> { using type = remove_all<T>::type; };

	template<typename T>
	using remove_all_t = remove_all<T>::type;

	template<typename T> struct add_const_on_value_type { typedef const T type; };
	template<typename T> struct add_const_on_value_type<T&> { typedef T const& type; };
	template<typename T> struct add_const_on_value_type<T*> { typedef T const* type; };
	template<typename T> struct add_const_on_value_type<T* const> { typedef T const* const type; };
	template<typename T> struct add_const_on_value_type<T const* const> { typedef T const* const type; };

	template<typename T>
	using add_const_on_value_type_t = typename add_const_on_value_type<T>::type;
}