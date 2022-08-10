/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <cstdlib>

namespace LCH::Math
{
	// 对齐数组
	template<typename T, int Size, int Options, 
		int Alignment = (Options & DontAlign) ? 0 : SimdInstruction<T, Size>::Alignment>
	struct PlainArray
	{
		alignas(Alignment) T array[Size];
	};

	// 非对齐数组
	template<typename T, int Size, int Options>
	struct PlainArray<T, Size, Options, 0>
	{
		T array[Size];
	};

	template<typename T, bool Align>
	inline void* conditional_aligned_alloc(size_t size)
	{
		return nullptr;
	}

	template<typename T, int Size, int Rows, int Cols, int Options> 
	class Storage
	{
	public:
		T const* data() const { return m_data.array; }
		int rows() { return Rows; }
		int cols() { return Cols; }

	private:
		PlainArray<T, Size, Options> m_data;
	};

	template<typename T, int Options>
	class Storage<T, Dynamic, Dynamic, Dynamic, Options>
	{
	public:
		Storage() : m_data(nullptr), m_rows(0), m_cols(0) {}

		void resize(int size, int rows, int cols)
		{
			if (size != m_rows * m_cols)
			{

			}
			m_rows = rows;
			m_cols = cols;
		}

		T const* data() const { return m_data; }
		int rows() { return m_rows; }
		int cols() { return m_cols; }

	private:
		T* m_data;
		int m_rows;
		int m_cols;
	};
}