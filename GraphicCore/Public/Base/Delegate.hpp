/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Exception/BaseException.h"
#include <vector>
#include <functional>
#include <mutex>
#include <algorithm>

namespace TG
{
    class DelegateHandle
    {
    public:
        enum GenerateNewHandleType
        {
            GenerateNewHandle
        };

        DelegateHandle() = default;
        explicit DelegateHandle(GenerateNewHandleType) : m_id(GenerateNewID()) {}

        bool operator==(const DelegateHandle& Rhs) const
        {
            return m_id == Rhs.m_id;
        }
        bool operator!=(const DelegateHandle& Rhs) const
        {
            return m_id != Rhs.m_id;
        }

        [[nodiscard]] bool IsValid() const
        {
            return m_id != 0;
        }

        void Refresh()
        {
            m_id = GenerateNewID();
        }

        void Reset()
        {
            m_id = 0;
        }

    private:
        static uint64_t GenerateNewID();

        uint64_t m_id{ 0 };
    };

    /**
     * \brief 必须使用辅助结构体来表示类成员函数的类型
     *        否则Delegate的Bind在绑定类成员函数时，如果没有传递VarTypes，编译无法通过，找不到重载函数
     *        但是静态函数或普通函数可以直接用RetType(*function)(Args..., std::decay_t<VarTypes>...)，
     *        这里先记录一下，以后再仔细查原因
     *        这里的辅助结构体参考了UE的实现
     */
    template <bool Const, typename Class, typename FuncType>
    struct MemFunPtrType;

    template <typename Class, typename RetType, typename... ArgTypes>
    struct MemFunPtrType<false, Class, RetType(ArgTypes...)>
    {
        typedef RetType(Class::*Type)(ArgTypes...);
    };

    template <typename Class, typename RetType, typename... ArgTypes>
    struct MemFunPtrType<true, Class, RetType(ArgTypes...)>
    {
        typedef RetType(Class::*Type)(ArgTypes...) const;
    };

    template <typename Ret, typename... Args>
    class Delegate;

    // 委托
    template <typename RetType, typename... Args>
    class Delegate<RetType(Args...)>
    {
    public:
        Delegate() = default;

        template <typename Functor, typename... VarTypes>
        requires std::is_invocable_v<Functor, Args..., VarTypes...>
        Delegate(Functor&& functor, VarTypes... vars)
            : m_delegateHandle(DelegateHandle::GenerateNewHandle),
              m_delegate([functor, ...capturedArgs = vars](Args... args) {
                std::invoke(functor, std::forward<Args>(args)..., capturedArgs...);
            })
        {}

        template <typename Functor>
        Delegate& operator=(Functor&& functor)
        {
            m_delegateHandle.Refresh();
            m_delegate = std::forward<Functor>(functor);
            return *this;
        }

        // 绑定普通函数或静态函数
        // 绑定重载函数时，这个Bind可以绑定到指定的函数
        template <typename... VarTypes>
        void Bind(RetType(*function)(Args..., std::decay_t<VarTypes>...), VarTypes... vars)
        {
            if (function == nullptr)
                return;

            m_delegateHandle.Refresh();
            m_delegate = [function, ...capturedArgs = vars](Args... args) {
                return std::invoke(function, std::forward<Args>(args)..., capturedArgs...);
            };
        }
        // 绑定类成员函数
        template <typename T, typename... VarTypes>
        void Bind(typename MemFunPtrType<false, T, RetType(Args..., std::decay_t<VarTypes>...)>::Type method,
            T* object, VarTypes... vars)
        {
            if (method == nullptr || object == nullptr)
                return;

            m_delegateHandle.Refresh();
            m_delegate = [object, method, ...capturedArgs = vars](Args... args) {
                return std::invoke(method, object, std::forward<Args>(args)..., capturedArgs...);
            };
        }
        template <typename T, typename... VarTypes>
        void Bind(typename MemFunPtrType<true, T, RetType(Args..., std::decay_t<VarTypes>...)>::Type method,
            T const* object, VarTypes... vars)
        {
            if (method == nullptr || object == nullptr)
                return;

            m_delegateHandle.Refresh();
            m_delegate = [object, method, ...capturedArgs = vars](Args... args) {
                return std::invoke(method, object, std::forward<Args>(args)..., capturedArgs...);
            };
        }
        // 绑定lambda表达式
        template <typename Functor, typename... VarTypes>
        requires std::is_invocable_v<Functor, Args..., VarTypes...>
        void Bind(Functor&& functor, VarTypes... vars)
        {
            m_delegateHandle.Refresh();
            m_delegate = [functor, ...capturedArgs = vars](Args... args) {
                return std::invoke(functor, std::forward<Args>(args)..., capturedArgs...);
            };
        }

        void UnBind()
        {
            m_delegateHandle.Reset();
            m_delegate = nullptr;
        }

        [[nodiscard]] DelegateHandle GetHandle() const { return m_delegateHandle; };

        bool operator==(const Delegate& Rhs) const
        {
            return m_delegateHandle == Rhs.m_delegateHandle;
        }

        bool operator!=(const Delegate& Rhs) const
        {
            return m_delegateHandle != Rhs.m_delegateHandle;
        }

        explicit operator bool() const
        {
            return m_delegateHandle.IsValid();
        }

        RetType Execute(Args... args) const
        {
            if (!m_delegateHandle.IsValid())
                throw BaseException::Create("Invalid delegate!");

            return std::invoke(m_delegate, std::forward<Args>(args)...);
        }

        bool ExecuteIfBound(Args... args) const
        {
            if (!m_delegateHandle.IsValid())
                return false;

            std::invoke(m_delegate, std::forward<Args>(args)...);
            return true;
        }

    private:
        DelegateHandle m_delegateHandle;
        std::function<RetType(Args...)> m_delegate;
    };

    template <typename... Args>
    class MulticastDelegate;

    // 多播委托
    template <typename... Args>
    class MulticastDelegate<void(Args...)>
    {
        using Delegate = Delegate<void(Args...)>;

    public:
        void Add(const Delegate& delegate)
        {
            if (delegate)
                m_delegates.emplace_back(delegate);
        }

        void Add(Delegate&& delegate)
        {
            if (delegate)
                m_delegates.emplace_back(std::forward<Delegate>(delegate));
        }
        // 参考Delegate的实现
        template <typename... VarTypes>
        DelegateHandle Add(void(*function)(Args..., std::decay_t<VarTypes>...), VarTypes... vars)
        {
            if (function == nullptr)
                return {};

            Delegate delegate;
            delegate.Bind(function, vars...);
            const DelegateHandle handle = delegate.GetHandle();
            m_delegates.emplace_back(std::move(delegate));
            return handle;
        }
        // 绑定类成员函数
        template <typename T, typename... VarTypes>
        DelegateHandle Add(typename MemFunPtrType<false, T, void(Args..., std::decay_t<VarTypes>...)>::Type method,
            T* object, VarTypes... vars)
        {
            if (method == nullptr || object == nullptr)
                return {};

            Delegate delegate;
            delegate.Bind(method, object, vars...);
            const DelegateHandle handle = delegate.GetHandle();
            m_delegates.emplace_back(std::move(delegate));
            return handle;
        }
        template <typename T, typename... VarTypes>
        DelegateHandle Add(typename MemFunPtrType<true, T, void(Args..., std::decay_t<VarTypes>...)>::Type method,
            T const* object, VarTypes... vars)
        {
            if (method == nullptr || object == nullptr)
                return {};

            Delegate delegate;
            delegate.Bind(method, object, vars...);
            const DelegateHandle handle = delegate.GetHandle();
            m_delegates.emplace_back(std::move(delegate));
            return handle;
        }
        // 绑定lambda表达式
        template <typename Functor, typename... VarTypes>
        requires std::is_invocable_v<Functor, Args..., VarTypes...>
        DelegateHandle Add(Functor&& functor, VarTypes... vars)
        {
            Delegate delegate(functor, vars...);
            const DelegateHandle handle = delegate.GetHandle();
            m_delegates.emplace_back(std::move(delegate));
            return handle;
        }

        bool Remove(DelegateHandle handle)
        {
            auto result = std::ranges::remove_if(
                m_delegates,
                [handle](const Delegate& delegate) {
                    return delegate.GetHandle() == handle;
                }
            );
            if (result.begin() == m_delegates.end())
                return false;

            m_delegates.erase(result.begin(), result.end());
            return true;
        }

        bool Remove(const Delegate& delegate)
        {
            auto result = std::ranges::remove(m_delegates, delegate);
            if (result.begin() == m_delegates.end())
                return false;

            m_delegates.erase(result.begin(), result.end());
            return true;
        }

        void Clear()
        {
            m_delegates.clear();
        }

        void Broadcast(Args... args)
        {
            for (const auto& delegate : m_delegates)
                delegate.Execute(std::forward<Args>(args)...);
        }

    private:
        std::vector<Delegate> m_delegates;
    };

    // 线程安全多播委托
    // template <typename... Args>
    // class TSMulticastDelegate : public MulticastDelegate<void(Args...)>
    // {
    //     using Super = MulticastDelegate<void(Args...)>;
    //
    // public:
    //     void Add()
    //     {
    //         std::lock_guard lock(m_mutex);
    //         Super::Add();
    //     }
    //
    //     void Remove()
    //     {
    //         std::lock_guard lock(m_mutex);
    //         Super::Remove();
    //     }
    //
    //     void Clear()
    //     {
    //         std::lock_guard lock(m_mutex);
    //         Super::Clear();
    //     }
    //
    //     void Broadcast(Args... args)
    //     {
    //         std::lock_guard lock(m_mutex);
    //         Super::Broadcast(std::forward<Args>(args)...);
    //     }
    //
    // private:
    //     std::mutex m_mutex;
    // };
}
