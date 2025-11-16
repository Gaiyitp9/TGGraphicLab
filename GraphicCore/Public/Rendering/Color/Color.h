/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"

namespace TG::Rendering
{
	class Color
	{
	public:
		Color() = default;
		Color(float r, float g, float b);
		explicit Color(float c);
		explicit Color(const Math::Vector3f& color);
		explicit Color(const Math::Vector4f& color);

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
		[[nodiscard]] Math::Vector4f ToVector4() const;

		Color operator*(const Color& color) const;
		Color operator*(float c) const;

	    Color operator+(const Color& color) const;
	    Color& operator+=(const Color& color);
	    Color& operator/=(float c);

	    Color LinearToGamma(float gamma = 2.2f);
	    Color GammaToLinear(float gamma = 2.2f);

	    static Color Random(float min = 0.0f, float max = 1.0f);

	private:
		Math::Vector4f m_channels;
	};

	inline Color operator*(float c, const Color& color)
	{
		return color * c;
	}
}
