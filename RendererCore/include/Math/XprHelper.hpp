/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <type_traits>

namespace LCH::Math
{
	template<typename T> struct remove_all { using type = T; };
	template<typename T> struct remove_all<const T>		{ using type = remove_all<T>::type; };
	template<typename T> struct remove_all<const T&>	{ using type = remove_all<T>::type; };
	template<typename T> struct remove_all<T&>			{ using type = remove_all<T>::type; };
	template<typename T> struct remove_all<T const*>	{ using type = remove_all<T>::type; };
	template<typename T> struct remove_all<T*>			{ using type = remove_all<T>::type; };

	template<typename T>
	using remove_all_t = remove_all<T>::type;

	template<typename T>
	struct ref_selector
	{
		using type = std::conditional_t<traits<T>::Flags, const T&, const T>;
		using non_const_type = std::conditional_t<traits<T>::Flags, T&, T>;
	};

	template<typename ScalarA, typename ScalarB, typename BinaryOp = scalar_sum_op<ScalarA, ScalarB>>
	struct scalar_binaryop_traits;

	template<typename T, typename BinaryOp>
	struct scalar_binaryop_traits<T, T, BinaryOp>
	{
		using return_type = T;
	};
}