/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	template<typename Scalar>
	struct NumTraits
	{
		constexpr static Scalar Epsilon = static_cast<Scalar>(1e-6);
	};
}