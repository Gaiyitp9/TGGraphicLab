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

template<typename LhsScalar, typename RhsScalar>
struct scalar_sum_op
{
	using result_type = scalar_binaryop_traits<LhsScalar, RhsScalar, scalar_sum_op>::return_type;

	result_type operator()(const LhsScalar& a, const RhsScalar& b) const { return a + b; }

	template<typename Packet>
	Packet packetOp(const Packet& a, const Packet& b) const { return padd(a, b); }
};

template<> inline bool scalar_sum_op<bool, bool>::operator()(const bool& a, const bool& b) const { return a || b; };

template<typename LhsScalar, typename RhsScalar>
struct scalar_sub_op
{
	using result_type = scalar_binaryop_traits<LhsScalar, RhsScalar, scalar_sub_op>::return_type;

	result_type operator()(const LhsScalar& a, const RhsScalar& b) const { return a - b; }

	template<typename Packet>
	Packet packetOp(const Packet& a, const Packet& b) const { return psub(a, b); }
};

template<typename LhsScalar, typename RhsScalar>
struct scalar_product_op
{
	using result_type = scalar_binaryop_traits<LhsScalar, RhsScalar, scalar_product_op>::return_type;

	result_type operator()(const LhsScalar& a, const RhsScalar& b) const { return a * b; }

	template<typename Packet>
	Packet packetOp(const Packet& a, const Packet& b) const { return pmul(a, b); }
};

template<> inline bool scalar_product_op<bool, bool>::operator()(const bool& a, const bool& b) const { return a && b; };
}