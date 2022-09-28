/************************************************************************
* This file is part of Eigen, a lightweight C++ template library		*
* for linear algebra.													*
*																		*
* Copyright (C) 2011 Benoit Jacob <jacob.benoit.1@gmail.com>			*
* Copyright (C) 2011-2014 Gael Guennebaud <gael.guennebaud@inria.fr>	*
* Copyright (C) 2011-2012 Jitse Niesen <jitse@maths.leeds.ac.uk>		*
*																		*
* This Source Code Form is subject to the terms of the Mozilla			*
* Public License v. 2.0. If a copy of the MPL was not distributed		*
* with this file, You can obtain one at http://mozilla.org/MPL/2.0/.	*
*																		*
* Noted: I made some modifications in this file.						*
*************************************************************************/
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