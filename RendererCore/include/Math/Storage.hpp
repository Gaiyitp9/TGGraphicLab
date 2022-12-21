/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <cassert>

namespace TG::Math
{

	// 对齐数组
	template<typename T, int Size, int Alignment = default_alignment<T, Size>>
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
	// 手动分配对齐内存的原理: 分配size+alignment的内存空间，分为两部分，后一部分的起始地址与alignment对齐，
	// 该地址也是返回的地址；前一部分记录内存地址，用来释放内存块。
	// 注: c++标准明确要求动态分配的内存必须满足一定的对齐要求，一般是16字节对齐
	// https://stackoverflow.com/questions/59098246/why-is-dynamically-allocated-memory-always-16-bytes-aligned
	// 而对齐要求是16或32，所以内存块的前一部分的大小一定大于一个指针的大小
	inline void* handmade_aligned_malloc(std::size_t size, std::size_t alignment)
	{
		assert(alignment >= sizeof(void*) && (alignment & (alignment - 1)) == 0 && "Alignment must be at least sizeof(void*) and a power of 2");

		void* original = std::malloc(size + alignment);
		if (original == nullptr) 
			return nullptr;
		void* aligned = reinterpret_cast<void*>((reinterpret_cast<size_t>(original) & ~(size_t(alignment - 1))) + alignment);
		*(reinterpret_cast<void**>(original) - 1) = original;
		return aligned;
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

	template<typename T, int Size, int Rows, int Cols> 
	class Storage
	{
	public:
		Storage()
		{
			std::memset(m_data.array, 0, Size * sizeof(T));
		}
		Storage(const Storage& other)
		{
			std::memcpy(m_data.array, other.m_data.array, Size * sizeof(T));
		}
		~Storage() = default;

	public:
		const T& operator[](size_t index) const 
		{
			if (index >= Size)
				ThrowBaseExcept(L"Index Out of Range");
			return m_data.array[index]; 
		}
		T& operator[](size_t index) 
		{ 
			if (index >= Size)
				ThrowBaseExcept(L"Index Out of Range");
			return m_data.array[index]; 
		}
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
		Storage(const Storage& other)
		{
			std::memcpy(m_data, other.m_data, m_size * sizeof(T));
		}
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

		const T& operator[](size_t index) const
		{
			if(index >= m_size)
				ThrowBaseExcept(L"Index Out of Range");
			return m_data.array[index];
		}
		T& operator[](size_t index)
		{
			if (index >= m_size)
				ThrowBaseExcept(L"Index Out of Range");
			return m_data.array[index];
		}

		T const* data() const { return m_data; }
		T* data() { return m_data; }
		int rows() const noexcept { return m_rows; }
		int cols() const noexcept { return m_cols; }

	private:
		T* m_data;
		int m_rows;
		int m_cols;
		int m_size;
	};
}