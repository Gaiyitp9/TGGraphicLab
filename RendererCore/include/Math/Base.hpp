/********************************************************************
* TianGong RenderLab												*
* Copyright (c) Gaiyitp9. All rights reserved.						*
* This code is licensed under the MIT License (MIT).				*
*																	*
* Noted:															*
* This file is part of Eigen, a lightweight C++ template library	*
* for linear algebra which is subject to the terms of the			*
* Mozilla Public License v.2.0. And I made some simplifications.	*
*********************************************************************/
#pragma once

namespace LCH::Math
{

template<typename Derived>
struct Base
{
	Derived& derived() { return *static_cast<Derived*>(this); }
	const Derived& derived() const { return *static_cast<Derived const*>(this); }
	const Derived& const_derived() const { return *static_cast<Derived const*>(this); }
	Derived& const_cast_derived() const { return *static_cast<Derived*>(const_cast<Base*>(this)); }

	int rows() const { return derived().rows(); }
	int cols() const { return derived().cols(); }
	int size() const { return rows() * cols(); }
};

}