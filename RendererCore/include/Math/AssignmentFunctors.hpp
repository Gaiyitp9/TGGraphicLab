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

template<typename DstScalar, typename SrcScalar>
struct assign_op
{
	void AssignCoeff(DstScalar& a, const SrcScalar& b) const { a = b; }

	template<int Alignment, typename Packet>
	void AssignPacket(DstScalar* a, const Packet& b) const
	{
		pstoret<DstScalar, Packet, Alignment>(a, b);
	}
};

template<typename DstScalar, typename SrcScalar>
struct functor_traits<assign_op<DstScalar, SrcScalar>>
{
	constexpr static bool PacketAccess = std::is_same_v<DstScalar, SrcScalar> 
										&& packet_traits<DstScalar>::Vectorizable 
										&& packet_traits<SrcScalar>::Vectorizable;
};

}