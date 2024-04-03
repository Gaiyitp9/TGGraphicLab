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
		ColumnMajor,
	};

#ifdef TG_ROW_MAJOR_MATRIX
	inline constexpr StorageOption DefaultMatrixStorageOrderOption = StorageOption::RowMajor;
#else
	inline constexpr StorageOption DefaultMatrixStorageOrderOption = StorageOption::ColumnMajor;
#endif

	// 矩阵类
	template<typename ScalarT, int Rows, int Cols,
		StorageOption Options_ = (Rows == 1 && Cols != 1 ? StorageOption::RowMajor :
			Rows != 1 && Cols == 1 ? StorageOption::ColumnMajor :
            DefaultMatrixStorageOrderOption)
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

	// 根据数据类型选择数学函数
	template<typename Scalar> inline Scalar sqrt(Scalar x) { return std::sqrt(x); }
	template<> inline float sqrt(float x) { return std::sqrtf(x); }
	template<> inline long double sqrt(long double x) { return std::sqrtl(x); }

	template<typename Scalar> inline Scalar sin(Scalar x) { return std::sin(x); }
	template<> inline float sin(float x) { return std::sinf(x); }
	template<> inline long double sin(long double x) { return std::sinl(x); }

	template<typename Scalar> inline Scalar cos(Scalar x) { return std::cos(x); }
	template<> inline float cos(float x) { return std::cosf(x); }
	template<> inline long double cos(long double x) { return std::cosl(x); }

	template<typename Scalar> inline Scalar tan(Scalar x) { return std::tan(x); }
	template<> inline float tan(float x) { return std::tanf(x); }
	template<> inline long double tan(long double x) { return std::tanl(x); }
}