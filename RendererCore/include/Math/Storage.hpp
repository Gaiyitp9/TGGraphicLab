/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{
	// 对齐数组
	template<typename T, int Size, int Options, 
		int Alignment = (Options & DontAlign) ? 0 : SimdInstruction<T, Size>::Alignment>
	struct plain_array
	{
		T array[Size];
	};

	// 非对齐数组
	template<typename T, int Size, int Options>
	struct plain_array<T, Size, Options, 16>
	{
		alignas(16) T array[Size];

		plain_array()
		{

		}
	};

	template<typename T, int Size, int Rows, int Cols, int Options> 
	class Storage
	{
	public:
		T const* data() const { return m_data; }

	private:
		T[Size] m_data;
	};

	template<typename T, int Options>
	class Storage<T, Dynamic, Dynamic, Dynamic, Options>
	{
	public:
		Storage(){}

		void resize()
		{

		}

	private:
		T* m_data;
	};
}