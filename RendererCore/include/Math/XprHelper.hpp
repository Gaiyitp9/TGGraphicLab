/************************************************************************
* This file is part of Eigen, a lightweight C++ template library		*
* for linear algebra.													*
*																		*
* Copyright (C) 2011 Benoit Jacob <jacob.benoit.1@gmail.com>			*
* Copyright (C) 2011-2014 Gael Guennebaud <gael.guennebaud@inria.fr>	*
* Copyright (C) 2011-2012 Jitse Niesen <jitse@maths.leeds.ac.uk>		*
*																		*
* This Source Code Form is subject to the terms of the Mozilla			*
* Public License v. 2.0. If a copy of the MPL was not distributed		*
* with this file, You can obtain one at http://mozilla.org/MPL/2.0/.	*
*																		*
* Noted: I made some modifications in this file.						*
*************************************************************************/
#pragma once

namespace LCH::Math
{

// 移除类型的const、引用和指针，获得纯类型
template<typename T> struct remove_all { using type = T; };
template<typename T> struct remove_all<const T>		{ using type = remove_all<T>::type; };
template<typename T> struct remove_all<const T&>	{ using type = remove_all<T>::type; };
template<typename T> struct remove_all<T&>			{ using type = remove_all<T>::type; };
template<typename T> struct remove_all<T const*>	{ using type = remove_all<T>::type; };
template<typename T> struct remove_all<T*>			{ using type = remove_all<T>::type; };

template<typename T>
using remove_all_t = remove_all<T>::type;

// 添加const修饰符
template<typename T> struct add_const_on_value_type { typedef const T type; };
template<typename T> struct add_const_on_value_type<T&> { typedef T const& type; };
template<typename T> struct add_const_on_value_type<T*> { typedef T const* type; };
template<typename T> struct add_const_on_value_type<T* const> { typedef T const* const type; };
template<typename T> struct add_const_on_value_type<T const* const> { typedef T const* const type; };

template<typename T>
using add_const_on_value_type_t = typename add_const_on_value_type<T>::type;

// 选择变量声明方式
template<typename T>
struct ref_selector
{
	using type = std::conditional_t<not_none(traits<T>::Flags & Flag::NestByRef), const T&, const T>;
	using non_const_type = std::conditional_t<not_none(traits<T>::Flags & Flag::NestByRef), T&, T>;
};

// 变量对应的包特性(包中有多个变量，表示SIMD中使用的变量，比如__m128)
template<typename T> struct packet_traits;
// 包的特性(通过包获得特性，上面是通过变量获得)
template<typename T> struct unpacket_traits;

// 寻找最合适的包，核心思路是尽可能使用SIMD，所以根据包的尺寸要是Size的整数倍
// 如果不满足，就检查半包(半包指的是包尺寸一半的包，比如__m128是__m256的半包)
// 所以按以下条件按顺序判断，满足就停止寻找: 
// 1. 动态矩阵 2. 矩阵尺寸是包尺寸的整数倍 3. 包类型与半包类型一致
template<int Size, typename PacketType, 
		 bool Stop = Size == Dynamic || (Size%unpacket_traits<PacketType>::Size)==0 || std::is_same<PacketType, typename unpacket_traits<PacketType>::half>::value>
struct find_best_packet_helper;

template<int Size, typename PacketType>
struct find_best_packet_helper<Size, PacketType, true>
{
	using type = PacketType;
};

template<int Size, typename PacketType>
struct find_best_packet_helper<Size, PacketType, false>
{
	using type = find_best_packet_helper<Size, typename unpacket_traits<PacketType>::half>::type;
};

template<typename T, int Size>
using best_packet = find_best_packet_helper<Size, typename packet_traits<T>::type>::type;

// 编译期计算矩阵尺寸
inline constexpr int size_at_compile_time(int rows, int cols)
{
	return (rows == Dynamic || cols == Dynamic) ? Dynamic : rows * cols;
}

inline constexpr int compute_default_alignment_helper(int arrayBytes, int alignmentBytes)
{
	if ((arrayBytes % alignmentBytes) == 0)
		return alignmentBytes;
	else if (MIN_ALIGN_BYTES < alignmentBytes)
		return compute_default_alignment_helper(arrayBytes, alignmentBytes / 2);
	else
		return 0;
}

// 计算默认对齐
// 为什么要根据矩阵尺寸(字节数)来计算默认对齐？我的理解是这样：如果矩阵尺寸不是16或32的整数倍，
// 分配对齐内存会产生浪费；另外，在AssignEvaluator中进行计算时，会单独处理不对齐的矩阵系数。
// 考虑只有3个float的向量，如果分配对齐内存，会浪费空间，直接在AssignEvaluator中处理不对齐的部分才是对的。
// 注意：只有固定尺寸的矩阵需要用这个来计算，动态矩阵的默认对齐为DEFAULT_ALIGN_BYTES。
template<typename T, int Size>
inline constexpr int default_alignment = compute_default_alignment_helper(Size * sizeof(T), MAX_ALIGN_BYTES);

// 判断是否是左值
template<typename XprType>
inline constexpr bool is_lvalue = (!std::is_const_v<XprType>)
								&& not_none(traits<XprType>::Flags & Flag::Lvalue);

// 两个矩阵表达式的尺寸是否一致
template<typename Type0, typename Type1>
inline constexpr bool have_same_matrix_size = (Type0::SizeAtCompileTime == 0 && Type1::SizeAtCompileTime == 0)
											|| ((Type0::RowsAtCompileTime == Dynamic ||
												Type1::RowsAtCompileTime == Dynamic ||
												Type0::RowsAtCompileTime == Type1::RowsAtCompileTime) 
												&& (Type0::ColsAtCompileTime == Dynamic ||
													Type1::ColsAtCompileTime == Dynamic ||
													Type0::ColsAtCompileTime == Type1::ColsAtCompileTime));

// 两个向量表达式的尺寸是否一致
template<typename Type0, typename Type1>
inline constexpr bool have_same_vector_size = (Type0::SizeAtCompileTime == Dynamic || 
												Type1::SizeAtCompileTime == Dynamic ||
												Type0::SizeAtCompileTime == Type1::SizeAtCompileTime);	

// 矩阵表达式步长
template<typename Derived, bool HasDirectAccess = has_direct_access<Derived>>
inline constexpr int inner_stride_at_compile_time = traits<Derived>::InnerStrideAtCompileTime;

template<typename Derived>
inline constexpr int inner_stride_at_compile_time<Derived, false> = 0;

template<typename Derived, bool HasDirectAccess = has_direct_access<Derived>>
inline constexpr int outer_stride_at_compile_time = traits<Derived>::OuterStrideAtCompileTime;

template<typename Derived>
inline constexpr int outer_stride_at_compile_time<Derived, false> = 0;
}