/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
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
	using type = std::conditional_t<(traits<T>::Flags & Flag::NestByRef) != Flag::None, const T&, const T>;
	using non_const_type = std::conditional_t<(traits<T>::Flags & Flag::NestByRef) != Flag::None, T&, T>;
};

// 二元运算符操作数的特性
template<typename ScalarA, typename ScalarB, typename BinaryOp = scalar_sum_op<ScalarA, ScalarB>>
struct scalar_binaryop_traits;

template<typename T, typename BinaryOp>
struct scalar_binaryop_traits<T, T, BinaryOp>
{
	using return_type = T;
};

// 获取可调用变量(比如函数或者lambada表达式)的返回值
template<typename Callable, typename... ArgTypes>
struct invoke_result_of
{
	typedef typename std::invoke_result<Callable, ArgTypes...>::type type1;
	typedef remove_all_t<type1> type;
};

template<typename T> 
struct functor_traits
{
	constexpr static bool PacketAccess = false;
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
template<typename T, int Size>
inline constexpr int default_alignment = compute_default_alignment_helper(Size * sizeof(T), MAX_ALIGN_BYTES);

// 判断是否是左值
template<typename ExpressionType>
inline constexpr bool is_lvalue = (!std::is_const_v<ExpressionType>) 
								&& (traits<ExpressionType>::Flags & Flag::Lvalue) != Flag::None;

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

struct meta_yes { char a[1]; };
struct meta_no { char a[2]; };
// 类型T是否包含return_type成员
template<typename T>
struct has_return_type
{
	template<typename C> static meta_yes TestFunctor(C const*, C::return_type const* = 0);
	template<typename C> static meta_no TestFunctor(...);

	static constexpr bool value = sizeof(TestFunctor<T>(static_cast<T*>(0))) == sizeof(meta_yes);
};

}