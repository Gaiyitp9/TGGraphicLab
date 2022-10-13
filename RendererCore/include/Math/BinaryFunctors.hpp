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

template<typename Scalar>
struct scalar_sum_op
{
	Scalar operator()(const Scalar& a, const Scalar& b) const { return a + b; }

	template<typename Packet>
	Packet packetOp(const Packet& a, const Packet& b) const { return padd(a, b); }
};

template<typename Scalar>
struct scalar_sub_op
{
	Scalar operator()(const Scalar& a, const Scalar& b) const { return a - b; }

	template<typename Packet>
	Packet packetOp(const Packet& a, const Packet& b) const { return psub(a, b); }
};

template<typename Scalar>
struct scalar_product_op
{
	Scalar operator()(const Scalar& a, const Scalar& b) const { return a * b; }

	template<typename Packet>
	Packet packetOp(const Packet& a, const Packet& b) const { return pmul(a, b); }
};

}