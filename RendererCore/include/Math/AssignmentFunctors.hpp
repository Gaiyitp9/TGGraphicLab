/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
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

	}
};

template<typename DstScalar, typename SrcScalar>
struct functor_traits<assign_op<DstScalar, SrcScalar>>
{
	constexpr static bool PacketAccess = std::is_same_v<DstScalar, SrcScalar> && packet_traits<DstScalar>::Vectorizable && packet_traits<SrcScalar>::Vectorizable;
};

}