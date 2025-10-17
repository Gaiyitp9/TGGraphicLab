/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Color/Color.h"

namespace TG::Color
{
    Color::Color(const Math::Vector4f& color)
    {
        m_channels = color;
    }

    Color::Color(float c)
    {
        m_channels[0] = m_channels[1] = m_channels[2] = c;
        m_channels[3] = 1.f;
    }

    Color::Color(float r, float g, float b)
    {
        m_channels[0] = r;
        m_channels[1] = g;
        m_channels[2] = b;
        m_channels[3] = 1.f;
    }

    Math::Vector4f Color::ToVector4() const
    {
        return { m_channels[0], m_channels[1], m_channels[2], m_channels[3] };
    }

    Color Color::operator*(const Color& color) const
    {
        return Color(m_channels.CWiseProduct(color.m_channels));
    }

    Color Color::operator*(float c) const
    {
        return Color(m_channels * c);
    }

    Color Color::operator+(const Color& color) const
    {
        return Color(m_channels + color.m_channels);
    }
}
