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

template<typename LhsScalar, typename RhsScalar>
struct scalar_sum_op
{
	using result_type = scalar_binaryop_traits<LhsScalar, RhsScalar, scalar_sum_op>::return_type;

	result_type operator()(const LhsScalar& a, const RhsScalar& b) const { return a + b; }

	template<typename Packet>
	Packet packetOp(const Packet& a, const Packet& b) const { return padd(a, b); }
};

template<typename LhsScalar, typename RhsScalar>
struct functor_traits<scalar_sum_op<LhsScalar, RhsScalar>>
{
	constexpr static bool PacketAccess = std::is_same_v<LhsScalar, RhsScalar> && 
										packet_traits<LhsScalar>::HasAdd &&
										packet_traits<RhsScalar>::HasAdd;
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
struct functor_traits<scalar_sub_op<LhsScalar, RhsScalar>>
{
	constexpr static bool PacketAccess = std::is_same_v<LhsScalar, RhsScalar> &&
		packet_traits<LhsScalar>::HasSub &&
		packet_traits<RhsScalar>::HasSub;
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