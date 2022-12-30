/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	template<typename T> struct traits;
	template<typename T> struct traits<const T> : traits<T> {};

	// 矩阵储存格式
	enum class StorageOption : char
	{
		RowMajor,
		ColMajor,
	};
	inline constexpr StorageOption DEFAULT_MATRIX_STORAGE_ORDER_OPTION = StorageOption::RowMajor;
	inline constexpr int DYNAMIC = -1;

	// 矩阵类
	template<typename ScalarT, int Rows, int Cols,
		StorageOption Options_ = (Rows == 1 && Cols != 1 ? StorageOption::RowMajor :
			Rows != 1 && Cols == 1 ? StorageOption::ColMajor :
			DEFAULT_MATRIX_STORAGE_ORDER_OPTION)
	> class Matrix;

	// 变量对应的包特性(包中有多个变量，表示SIMD中使用的变量，比如__m128)
	template<typename T>
	struct packet_traits
	{
		using type = T;
	};
	template<typename T> struct packet_traits<const T> : packet_traits<T> {};
	// 包的特性(通过包获得特性，上面是通过变量获得)
	template<typename T> struct unpacket_traits
	{
		using type = T;
		using half = T;
		constexpr static int Size = 1;			// 包尺寸
		constexpr static int Alignment = 1;
	};
	template<typename T> struct unpacket_traits<const T> : unpacket_traits<T> { };

	// 根据数据类型选择平方根函数
	template<typename Scalar> inline Scalar sqrt(Scalar val) { return std::sqrt(val); }
	template<> inline float sqrt(float val) { return std::sqrtf(val); }
	template<> inline long double sqrt(long double val) { return std::sqrtl(val); }
}