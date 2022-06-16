/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{
	template <typename T>
	class Quaternion
	{
	public:
		Quaternion();
		Quaternion(T x, T y, T z, T w);
	};
}