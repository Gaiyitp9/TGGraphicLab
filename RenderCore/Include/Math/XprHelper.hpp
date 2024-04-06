/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
    // 给枚举添加逻辑运算
	template<typename Enum> requires std::is_enum_v<Enum>
	constexpr auto UnderlyingTypeCast(Enum e) { return static_cast<std::underlying_type_t<Enum>>(e); }

	template<typename Enum> requires std::is_enum_v<Enum>
	constexpr Enum operator|(Enum left, Enum right)
	{
		return static_cast<Enum>(UnderlyingTypeCast(left) | UnderlyingTypeCast(right));
	}

	template<typename Enum> requires std::is_enum_v<Enum>
	constexpr Enum operator^(Enum left, Enum right)
	{
		return static_cast<Enum>(UnderlyingTypeCast(left) ^ UnderlyingTypeCast(right));
	}

	template<typename Enum> requires std::is_enum_v<Enum>
	constexpr Enum operator&(Enum left, Enum right)
	{
		return static_cast<Enum>(UnderlyingTypeCast(left) & UnderlyingTypeCast(right));
	}

	template<typename Enum> requires std::is_enum_v<Enum>
	constexpr Enum operator~(Enum e)
	{
		return static_cast<Enum>(~UnderlyingTypeCast(e));
	}

    // 地址是否满足对齐条件
    template<typename T>
    inline bool IsAligned(T const* value, size_t alignment)
    {
        return 0 == ((size_t)value & (alignment - 1));
    }
}