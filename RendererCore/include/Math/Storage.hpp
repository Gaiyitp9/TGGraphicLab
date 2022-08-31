/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <cassert>

namespace LCH::Math
{

// 对齐数组
template<typename T, int Size, int Alignment = compute_default_alignment<T, Size>::value>
struct PlainArray
{
	alignas(Alignment) T array[Size];
};

// 非对齐数组
template<typename T, int Size>
struct PlainArray<T, Size, 0>
{
	T array[Size];
};

// 分配对齐内存
inline void* handmade_aligned_malloc(std::size_t size, std::size_t alignment)
{
	assert(alignment >= sizeof(void*) && (alignment & (alignment - 1)) == 0 && "Alignment must be at least sizeof(void*) and a power of 2");

	void* original = std::malloc(size + alignment);
	if (original == nullptr) 
		return nullptr;
	void* aligned = reinterpret_cast<void*>((reinterpret_cast<size_t>(original) & ~(size_t(alignment - 1))) + alignment);
	*(reinterpret_cast<void**>(original) - 1) = original;
}

// 释放对齐内存
inline void handmade_aligned_free(void* ptr)
{
	if (ptr)
		std::free(*(reinterpret_cast<void**>(ptr) - 1));
}

// 根据条件分配动态内存(默认16字节对齐)
template<bool Align>
inline void* conditional_aligned_alloc(size_t size)
{
	return std::malloc(size);
}

template<>
inline void* conditional_aligned_alloc<true>(size_t size)
{
	return handmade_aligned_malloc(size, DEFAULT_ALIGN_BYTES);
}

// 根据条件释放内存
template<bool Align>
inline void conditional_aligned_free(void* ptr)
{
	if (ptr)
		std::free(ptr);
}

template<>
inline void conditional_aligned_free<true>(void* ptr)
{
	handmade_aligned_free(ptr);
}

// 返回第一个与Alignment对齐的数组索引
template<int Alignment, typename Scalar, typename Index>
inline int first_aligned(const Scalar* array, int size)
{
	const int ScalarSize = sizeof(Scalar);
	const int ScalarMask = ScalarSize - 1;
	const int AlignmentSize = Alignment / ScalarSize;
	const int AlignmentMask = AlignmentSize - 1;
	const size_t address = reinterpret_cast<size_t>(array);

	if (AlignmentSize <= 1)
		return 0;
	else if ((address & ScalarMask) || (Alignment & ScalarMask))
		return size;
	else
	{
		int first = (AlignmentSize - (address / ScalarSize) & AlignmentMask) & AlignmentMask;
		return (first < size) ? first : size;
	}
}

template<typename T, int Size, int Rows, int Cols> 
class Storage
{
public:
	const T& operator[](size_t index) const { return m_data.array[index]; }
	T& operator[](size_t index) { return m_data.array[index]; }
	T const* data() const { return m_data.array; }
	T* data() { return m_data.array; }
	constexpr static int rows() noexcept { return Rows; }
	constexpr static int cols() noexcept { return Cols; }

private:
	PlainArray<T, Size> m_data;
};

template<typename T>
class Storage<T, Dynamic, Dynamic, Dynamic>
{
public:
	Storage() : m_data(nullptr), m_rows(0), m_cols(0) {}
	~Storage() { conditional_aligned_free<true>(m_data); }

	void resize(int size, int rows, int cols)
	{
		if (size != m_rows * m_cols)
		{
			conditional_aligned_free<true>(m_data);
			if (size > 0)
				m_data = reinterpret_cast<T*>(conditional_aligned_alloc<true>(sizeof(T) * size));
			else
				m_data = nullptr;
		}
		m_rows = rows;
		m_cols = cols;
		m_size = size;
	}

	T const* data() const { return m_data; }
	int rows() const noexcept { return m_rows; }
	int cols() const noexcept { return m_cols; }

private:
	T* m_data;
	int m_rows;
	int m_cols;
	int m_size;
};

}