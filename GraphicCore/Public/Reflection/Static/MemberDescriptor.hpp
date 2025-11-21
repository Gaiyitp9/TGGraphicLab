/****************************************************************
* TianGong GraphicLab										*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    namespace Detail
    {
        template<typename Member>
        struct StaticFieldInvoker
        {
            static constexpr auto Invoke()
            {
                return *Member::Pointer;
            }

            template<typename U> requires (!Member::IsConst)
            static constexpr auto Invoke(U&& value)
            {
                return *Member::Pointer = std::forward<U>(value);
            }
        };

        template<typename Member>
        struct InstanceFieldInvoker
        {
            template<typename T>
            static constexpr auto Invoke(T&& target)
            {
                return target.*(Member::Pointer);
            }

            template<typename T, typename U> requires (!Member::IsConst)
            static constexpr auto Invoke(T&& target, U&& value)
            {
                return target.*(Member::Pointer) = std::forward<U>(value);
            }
        };
    }

    /**
     * \brief 类成员类型
     */
    enum class MemberType : unsigned char
    {
        Void,
        Field,
        Function
    };

    /**
     * \brief 判断类型T是否是类成员
     */
    template<typename T>
    concept IsMember = requires
    {
        { T::Type } -> std::same_as<MemberType>;
    };

    /**
     * \brief 判断类型T是否是字段
     */
    template<typename T>
    concept IsField = IsMember<T> && (T::Type == MemberType::Field);

    /**
     * \brief 判断类型T是否是函数
     */
    template<typename T>
    concept IsFunction = IsMember<T> && (T::Type == MemberType::Function);

    template<typename T, std::size_t N>
    struct MemberDescriptorBase
    {
        using Member = MemberInfo<T, N>;

        using DeclaringType = T;

        using AttributeTypes = AsTypeList<std::remove_cvref_t<decltype(Member::Attributes)>>;

        static constexpr auto Type{ MemberType::Void };

        static constexpr TypeDescriptor<T> Declarator{};

        static constexpr auto Name{ Member::Name };

        static constexpr auto Attributes{ Member::Attributes };
    };

    template<typename T, std::size_t N>
    struct FieldDescriptor : MemberDescriptorBase<T, N>
    {
        using typename MemberDescriptorBase<T, N>::Member;

        using ValueType = Member::ValueType;

        static constexpr bool IsStatic{ !std::is_member_object_pointer_v<decltype(Member::Pointer)> };

        static constexpr bool IsConst{ !std::is_const_v<ValueType> };

        static constexpr auto Pointer{ Member::Pointer };

        static constexpr auto Get() noexcept
        {
            return *Pointer;
        }

        template<typename U>
        static constexpr auto Get(U&& target) noexcept
        {
            return target.*(Pointer);
        }

        using Invoker = std::conditional_t<
            IsStatic,
            Detail::StaticFieldInvoker<FieldDescriptor>,
            Detail::InstanceFieldInvoker<FieldDescriptor>
        >;

        template<typename... Args>
        constexpr auto operator()(Args&&... args) const noexcept
        {
            return Invoker::Invoke(std::forward<Args>(args)...);
        }
    };

    template<typename T, std::size_t N>
    struct FunctionDescriptor : MemberDescriptorBase<T, N>
    {
        using typename MemberDescriptorBase<T, N>::Member;

        template<typename... Args>
        static constexpr auto Invoke(Args&&... args)
        {
            return Member::Invoke(std::forward<Args>(args)...);
        }

        template<typename... Args>
        using ReturnType = decltype(Member::Invoke(std::declval<Args>()...));

        template<typename... Args>
        constexpr auto operator()(Args&&... args) const
        {
            return Member::Invoke(std::forward<Args>(args)...);
        }

        static constexpr auto Pointer{ Member::Pointer() };
    };
}
