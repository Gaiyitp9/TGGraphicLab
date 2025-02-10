/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <unordered_map>
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
        unsigned long long Add(Delegate delegate)
        {
            if (auto result = m_delegates.try_emplace(delegate); result.second)
                return m_id++;

            return -1;
        }

        bool Remove(unsigned long long id)
        {
            return m_delegates.erase(id) > 0;
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
        inline static unsigned long long m_id{ 0 };
        std::unordered_map<unsigned long long, Delegate> m_delegates;
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
