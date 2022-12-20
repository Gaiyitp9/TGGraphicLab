/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{
	enum class StorageOption : char
	{
		RowMajor,
		ColMajor,
	};
	inline constexpr StorageOption DefaultMatrixStorageOrderOption = StorageOption::ColMajor;
	inline constexpr int Dynamic = -1;

	// 矩阵类
	template<typename ScalarT, int Rows, int Cols,
		StorageOption Options_ = (Rows == 1 && Cols != 1 ? StorageOption::RowMajor :
			Rows != 1 && Cols == 1 ? StorageOption::ColMajor :
			DefaultMatrixStorageOrderOption)
	> class Matrix;

	// 变量对应的包特性(包中有多个变量，表示SIMD中使用的变量，比如__m128)
	template<typename T> struct packet_traits;
	// 包的特性(通过包获得特性，上面是通过变量获得)
	template<typename T> struct unpacket_traits;

	// 寻找最合适的包，核心思路是尽可能使用SIMD，所以根据包的尺寸要是Size的整数倍
	// 如果不满足，就检查半包(半包指的是包尺寸一半的包，比如__m128是__m256的半包)
	// 所以按以下条件按顺序判断，满足就停止寻找: 
	// 1. 动态矩阵 2. 矩阵尺寸是包尺寸的整数倍 3. 包类型与半包类型一致
	template<int Size, typename PacketType,
		bool Stop = Size == Dynamic || (Size% unpacket_traits<PacketType>::Size) == 0 || std::is_same<PacketType, typename unpacket_traits<PacketType>::half>::value>
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
}