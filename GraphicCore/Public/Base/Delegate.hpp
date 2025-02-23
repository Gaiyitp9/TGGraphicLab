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
    template<typename ReturnType = void, typename... Args>
    class Delegate
    {
    public:
        void Bind(ReturnType(*function)(Args...))
        {
            m_delegate = function;
        }

        template<typename T>
        void Bind(T* object, ReturnType(T::*method)(Args...))
        {
            m_delegate = [object, method](Args... args) {
                return (object->*method)(std::forward<Args>(args)...);
            };
        }
        // 绑定lambda表达式
        void Bind(const std::function<ReturnType(Args...)>& lambda)
        {
            m_delegate = lambda;
        }

        ReturnType Execute(Args... args)
        {
            if (m_delegate)
                return m_delegate(std::forward<Args>(args)...);
            return {};
        }

    private:
        std::function<ReturnType(Args...)> m_delegate;
    };

    // 多播委托
    template<typename... Args>
    class MulticastDelegate
    {
    public:
        unsigned long long Add(void(*function)(Args...))
        {
            if (auto result = m_delegates.try_emplace(function); result.second)
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
        std::unordered_map<unsigned long long, std::function<void(Args...)>> m_delegates;
    };

    // 线程安全多播委托
    template<typename ReturnType, typename... Args>
    class TSMulticastDelegate : public MulticastDelegate<ReturnType(Args...)>
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
