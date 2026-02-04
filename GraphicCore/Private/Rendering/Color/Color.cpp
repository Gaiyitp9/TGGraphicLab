/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Rendering/Color/Color.h"
#include "Math/Random.hpp"

namespace TG::Rendering
{
    Color::Color(float r, float g, float b)
    {
        m_channels[0] = r;
        m_channels[1] = g;
        m_channels[2] = b;
        m_channels[3] = 1.f;
    }

    Color::Color(float c)
    {
        m_channels[0] = m_channels[1] = m_channels[2] = c;
        m_channels[3] = 1.f;
    }

    Color::Color(const Math::Vector3f& color)
    {
        m_channels.Block<3, 1>(0, 0) = color;
    }

    Color::Color(const Math::Vector4f& color)
    {
        m_channels = color;
    }

    Math::Vector4f Color::ToVector4() const
    {
        return { m_channels[0], m_channels[1], m_channels[2], m_channels[3] };
    }

    Color Color::operator*(const Color& color) const
    {
        return Color(Math::Vector4f(m_channels.CWiseProduct(color.m_channels)));
    }

    Color Color::operator*(float c) const
    {
        return Color(Math::Vector4f(m_channels * c));
    }

    Color Color::operator+(const Color& color) const
    {
        return Color(Math::Vector4f(m_channels + color.m_channels));
    }

    Color& Color::operator+=(const Color& color)
    {
        m_channels += color.m_channels;
        return *this;
    }

    Color& Color::operator/=(float c)
    {
        m_channels *= (1.0f / c);
        return *this;
    }

    Color Color::LinearToGamma(float gamma)
    {
        const float pow = 1.0f / gamma;
        float r = std::pow(m_channels[0], pow);
        float g = std::pow(m_channels[1], pow);
        float b = std::pow(m_channels[2], pow);
        return { r, g, b };
    }

    Color Color::GammaToLinear(float gamma)
    {
        float r = std::pow(m_channels[0], gamma);
        float g = std::pow(m_channels[1], gamma);
        float b = std::pow(m_channels[2], gamma);
        return { r, g, b };
    }

    Color Color::Random(float min, float max)
    {
        return {
            Math::g_random.Float(min, max),
            Math::g_random.Float(min, max),
            Math::g_random.Float(min, max)
        };
    }

}
