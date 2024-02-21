/****************************************************************
* TianGong Graphics Tool										*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    template<typename Func>
    class Method
    {
    public:
        explicit constexpr Method(Func&& func) : m_func(std::forward<Func>(func)) {}

        template<typename... Args>
        constexpr auto operator()(Args&&... args)
        {
            return std::invoke(m_func, std::forward<Args>(args)...);
        }

    private:
        Func m_func;
    };
}