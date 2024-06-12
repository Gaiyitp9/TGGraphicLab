/****************************************************************
* TianGong GraphicLab										*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    enum class SpecifierCategory : unsigned char
    {
        None,
        Extern,
        Static,
        ThreadLocal,
        Inline,
        Mutable,
        Const,
        Volatile,
        ConstExpr,
        ConstEval,
        ConstInit,
        Virtual,
        Private,
        Protected,
        Public
    };

    template<std::size_t N>
    class Specifier
    {
    public:
        constexpr Specifier(std::initializer_list<SpecifierCategory> init) : m_specifiers(init) {}

        [[nodiscard]] constexpr auto ToString() const
        {
            std::string out;
            for (auto& specifier : m_specifiers)
            {
                switch (specifier)
                {
                    case SpecifierCategory::None: out += " "; break;
                    case SpecifierCategory::Extern: out += "extern "; break;
                    case SpecifierCategory::Static: out += "static "; break;
                    case SpecifierCategory::ThreadLocal: out += "thread_local "; break;
                    case SpecifierCategory::Inline: out += "inline "; break;
                    case SpecifierCategory::Mutable: out += "mutable "; break;
                    case SpecifierCategory::Const: out += "const "; break;
                    case SpecifierCategory::Volatile: out += "volatile "; break;
                    case SpecifierCategory::ConstExpr: out += "constexpr "; break;
                    case SpecifierCategory::ConstEval: out += "consteval "; break;
                    case SpecifierCategory::ConstInit: out += "constinit "; break;
                    case SpecifierCategory::Virtual: out += "virtual "; break;
                    case SpecifierCategory::Private: out += "private "; break;
                    case SpecifierCategory::Protected: out += "protected "; break;
                    case SpecifierCategory::Public: out += "public "; break;
                }
            }
            return out.c_str();
        }

        [[nodiscard]] constexpr bool Contains(SpecifierCategory specifier) const
        {
            return std::ranges::any_of(m_specifiers, [&](SpecifierCategory s) { return s == specifier; });
        }

    private:
        std::array<SpecifierCategory, N> m_specifiers;
    };
}