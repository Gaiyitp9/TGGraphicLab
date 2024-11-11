/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <vector>
#include <functional>

namespace TG
{
    template<typename DelegateSignature>
    class Delegate;

    template<typename ReturnType, typename... Args>
    class Delegate<ReturnType(Args...)>
    {
        using Function = ReturnType(Args...);

    public:
        void BindRaw();

        ReturnType Execute(Args&&... args)
        {
            return m_function(std::forward<Args>(args)...);
        }

    private:
        std::function<Function> m_function;
    };

    template<typename DelegateSignature>
    class MulticastDelegate;

    template<typename ReturnType, typename... Args>
    class MulticastDelegate<ReturnType(Args...)>
    {
    public:
        void AddRaw();

        void Broadcast(Args&&... args)
        {
            for (auto delegate : m_delegates)
                delegate(std::forward<Args>(args)...);
        }

    private:
        std::vector<Delegate<ReturnType(Args...)>> m_delegates;
    };
}
