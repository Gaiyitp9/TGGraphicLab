/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"

namespace TG::Color
{
	class Color
	{
	public:
		Color() = default;
		explicit Color(const Math::Vector4F& color);
		explicit Color(float c);
		Color(float r, float g, float b);

		float operator[](std::size_t index) const
		{
			return m_channels[index];
		}

		float& operator[](std::size_t index)
		{
			return m_channels[index];
		}

		[[nodiscard]] float R() const { return m_channels[0]; }
		[[nodiscard]] float G() const { return m_channels[1]; }
		[[nodiscard]] float B() const { return m_channels[2]; }
		[[nodiscard]] float A() const { return m_channels[3]; }
		float& R() { return m_channels[0]; }
		float& G() { return m_channels[1]; }
		float& B() { return m_channels[2]; }
		float& A() { return m_channels[3]; }

		[[nodiscard]] float const* RGBA() const { return m_channels.Data(); }

		Color operator*(const Color& color) const;
		Color operator*(float c) const;

	private:
		Math::Vector4F m_channels;
	};

	inline Color operator*(float c, const Color& color)
	{
		return color * c;
	}
}
