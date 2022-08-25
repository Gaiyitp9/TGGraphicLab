/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace LCH::Math
{
	template<typename Derived>
	struct Base
	{
		Derived& derived() { return *static_cast<Derived*>(this); }
		const Derived& derived() const { return *static_cast<Derived const*>(this); }
		const Derived& const_derived() const { return *static_cast<Derived const*>(this); }

		int rows() const { return derived().rows(); }
		int cols() const { return derived().cols(); }
		int size() const { return rows() * cols(); }
	};
}