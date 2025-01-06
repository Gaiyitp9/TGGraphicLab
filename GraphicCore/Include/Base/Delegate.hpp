/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <vector>
#include <functional>
#include <mutex>

namespace TG
{
    // 委托
    template<typename DelegateSignature>
    class Delegate;

    // 多播委托
    template<typename DelegateSignature>
    class MulticastDelegate;

    template<typename ReturnType, typename... Args>
    class MulticastDelegate<ReturnType(Args...)>
    {
        using Delegate = std::function<ReturnType(Args...)>;

    public:
        void Add(Delegate delegate)
        {
            m_delegates.emplace_back(delegate);
        }

        void Remove()
        {

        }

        void Clear()
        {
            m_delegates.clear();
        }

        void Broadcast(Args&&... args)
        {
            for (const auto& delegate : m_delegates)
                delegate(std::forward<Args>(args)...);
        }

    private:
        std::vector<Delegate> m_delegates;
    };

    // 线程安全多播委托
    template<typename DelegateSignature>
    class TSMulticastDelegate;

    template<typename ReturnType, typename... Args>
    class TSMulticastDelegate<ReturnType(Args...)> : public MulticastDelegate<ReturnType(Args...)>
    {
        using Super = MulticastDelegate<ReturnType(Args...)>;

    public:
        void Add()
        {
            std::lock_guard lock(m_mutex);
            Super::Add();
        }

        void Remove()
        {
            std::lock_guard lock(m_mutex);
            Super::Remove();
        }

        void Clear()
        {
            std::lock_guard lock(m_mutex);
            Super::Clear();
        }

        void Broadcast(Args&&... args)
        {
            std::lock_guard lock(m_mutex);
            Super::Broadcast(std::forward<Args>(args)...);
        }

    private:
        std::mutex m_mutex;
    };
}
