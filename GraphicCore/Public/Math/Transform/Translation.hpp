/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"

namespace TG::Math
{
    template<typename Scalar, std::size_t Dimension> requires (Dimension == 2 || Dimension == 3)
    class Translation
    {
        using VectorType = Vector<Scalar, Dimension>;

    public:
        Translation() = default;
        Translation(Scalar x, Scalar y) requires (Dimension == 2)
            : m_translation{ x,  y }
        {}
        Translation(Scalar x, Scalar y, Scalar z) requires (Dimension == 3)
            : m_translation{ x, y, z }
        {}
        explicit Translation(const VectorType& v)
            : m_translation{ v }
        {}

        Scalar X() const { return m_translation.X(); }
        Scalar Y() const { return m_translation.Y(); }
        Scalar Z() const requires (Dimension == 3) { return m_translation.Z(); }

        Scalar& X() { return m_translation.X(); }
        Scalar& Y() { return m_translation.Y(); }
        Scalar& Z() requires (Dimension == 3) { return m_translation.Z(); }

        const VectorType& GetVector() const { return m_translation; }
        VectorType& GetVector() { return m_translation; }

    private:
        VectorType m_translation;
    };
}
