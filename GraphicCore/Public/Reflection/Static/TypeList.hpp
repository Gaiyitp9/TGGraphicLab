/****************************************************************
* TianGong GraphicLab										    *
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    namespace Detail
    {
        template<std::size_t N, typename... Ts>
        struct Get;
        template<std::size_t N, typename T, typename... Ts>
        struct Get<N, T, Ts...>
        {
            using Type = Get<N - 1, Ts...>::Type;
        };
        template<typename T, typename... Ts>
        struct Get<0, T, Ts...>
        {
            using Type = T;
        };
        // 超出类型列表的索引范围时，获取的类型为void
        template<std::size_t N>
        struct Get<N>
        {
            using Type = void;
        };

        template<std::size_t N, typename... Ts>
        struct Skip;
        template<std::size_t N, typename T, typename... Ts> requires (N <= sizeof...(Ts) + 1)
        struct Skip<N, T, Ts...>
        {
            using Type = Skip<N - 1, Ts...>::Type;
        };
        template<typename T, typename... Ts>
        struct Skip<0, T, Ts...>
        {
            using Type = TypeList<T, Ts...>;
        };
        // 已经跳过了所有类型，返回空列表
        template<std::size_t N>
        struct Skip<N>
        {
            using Type = TypeList<>;
        };

        template<std::size_t N, typename TypeList1, typename... Ts>
        struct TakeImplement;
        template<std::size_t N, typename T, typename... Ts, typename... Us>
        struct TakeImplement<N, TypeList<Us...>, T, Ts...>
        {
            using Type = TakeImplement<N - 1, TypeList<Us..., T>, Ts...>::Type;
        };
        template<typename T, typename... Ts, typename... Us>
        struct TakeImplement<0, TypeList<Us...>, T, Ts...>
        {
            using Type = TypeList<Us...>;
        };
        // 已经取完了所有类型，直接返回
        template<std::size_t N, typename... Us>
        struct TakeImplement<N, TypeList<Us...>>
        {
            using Type = TypeList<Us...>;
        };
        template<std::size_t N, typename... Ts>
        struct Take
        {
            using Type = TakeImplement<N, TypeList<>, Ts...>::Type;
        };

        template<typename...>
        struct Concat;
        template<>
        struct Concat<>
        {
            using Type = TypeList<>;
        };
        template<typename... Ts>
        struct Concat<TypeList<Ts...>>
        {
            using Type = TypeList<Ts...>;
        };
        template<typename... Ts, typename... Us>
        struct Concat<TypeList<Ts...>, TypeList<Us...>>
        {
            using Type = TypeList<Ts..., Us...>;
        };
        template<typename TypeList1, typename TypeList2, typename... TypeLists>
        struct Concat<TypeList1, TypeList2, TypeLists...>
        {
            using Type = Concat<typename Concat<TypeList1, TypeList2>::Type, TypeLists...>::Type;
        };

        template<typename T, typename TypeList1>
        struct Append
        {
            using Type = Concat<TypeList1, TypeList<T>>::Type;
        };

        template<typename T, typename TypeList1>
        struct Prepend
        {
            using Type = Concat<TypeList<T>, TypeList1>::Type;
        };

        template<typename TypeList1, typename TypeList2 = TypeList<>>
        struct Reverse;
        template<typename... Us>
        struct Reverse<TypeList<>, TypeList<Us...>>
        {
            using Type = TypeList<Us...>;
        };
        template<typename T, typename... Ts, typename... Us>
        struct Reverse<TypeList<T, Ts...>, TypeList<Us...>>
        {
            using Type = Reverse<TypeList<Ts...>, TypeList<T, Us...>>::Type;
        };

        template<template<typename> typename Predicate, typename... Ts>
        struct FilterByTypeTrait;
        template<template<typename> typename Predicate>
        struct FilterByTypeTrait<Predicate>
        {
            using Type = TypeList<>;
        };
        template<template<typename> typename Predicate, typename T, typename... Ts>
        requires IsCustomTypeTrait<Predicate, T>
        struct FilterByTypeTrait<Predicate, T, Ts...>
        {
            using Type = std::conditional_t<Predicate<T>::Value,
                typename Prepend<T, typename FilterByTypeTrait<Predicate, Ts...>::Type>::Type,
                typename FilterByTypeTrait<Predicate, Ts...>::Type
            >;
        };
        template<template<typename> typename Predicate, typename T, typename... Ts>
        struct FilterByTypeTrait<Predicate, T, Ts...>
        {
            using Type = std::conditional_t<Predicate<T>::value,
                typename Prepend<T, typename FilterByTypeTrait<Predicate, Ts...>::Type>::Type,
                typename FilterByTypeTrait<Predicate, Ts...>::Type
            >;
        };

        template<typename T, bool... Bs>
        struct ApplyMask;
        template<>
        struct ApplyMask<TypeList<>>
        {
            using Type = TypeList<>;
        };
        template<typename T, typename... Ts, bool... Bs>
        struct ApplyMask<TypeList<T, Ts...>, true, Bs...>
        {
            using Type = Prepend<T, typename ApplyMask<TypeList<Ts...>, Bs...>::Type>::Type;
        };
        template<typename T, typename... Ts, bool... Bs>
        struct ApplyMask<TypeList<T, Ts...>, false, Bs...>
        {
            using Type = ApplyMask<TypeList<Ts...>, Bs...>::Type;
        };

        template<template<typename> typename Mapper, typename... Ts>
        struct Map;
        template<template<typename> typename Mapper>
        struct Map<Mapper>
        {
            using Type = TypeList<>;
        };
        template<template<typename> typename Mapper, typename T, typename... Ts>
        requires IsCustomTypeTrait<Mapper, T>
        struct Map<Mapper, T, Ts...>
        {
            using Type = Prepend<typename Mapper<T>::Type, typename Map<Mapper, Ts...>::Type>::Type;
        };
        template<template<typename> typename Mapper, typename T, typename... Ts>
        struct Map<Mapper, T, Ts...>
        {
            using Type = Prepend<typename Mapper<T>::type, typename Map<Mapper, Ts...>::Type>::Type;
        };

        template<typename T, std::int64_t N, typename... Ts>
        constexpr std::int64_t IndexOf() noexcept
        {
            if constexpr (sizeof...(Ts) <= N)
                return -1;
            else if constexpr (std::is_same_v<T, std::tuple_element_t<N, std::tuple<Ts...>>>)
                return N;
            else
                return IndexOf<T, N + 1, Ts...>();
        }

        template<template<typename...> class T, std::int64_t N, typename... Ts>
        constexpr std::int64_t IndexOfInstance() noexcept
        {
            if constexpr (sizeof...(Ts) <= N)
                return -1;
            else if constexpr (Reflection::IsInstanceOf<T, std::tuple_element_t<N, std::tuple<Ts...>>>)
                return N;
            else
                return IndexOfInstance<T, N + 1, Ts...>();
        }

        template<typename T, std::int64_t N, typename... Ts>
        constexpr std::int64_t IndexOfBase() noexcept
        {
            if constexpr (sizeof...(Ts) <= N)
                return -1;
            else if constexpr (std::is_base_of_v<T, std::tuple_element_t<N, std::tuple<Ts...>>>)
                return N;
            else
                return IndexOfBase<T, N + 1, Ts...>();
        }

        template<typename... Ts>
        struct UniqueImplement;
        template<typename UniqueTypeList>
        struct UniqueImplement<UniqueTypeList>
        {
            using Type = UniqueTypeList;
        };
        template<typename UniqueTypeList, typename T, typename... Ts>
        struct UniqueImplement<UniqueTypeList, T, Ts...>
        {
            using Type = std::conditional_t<
                UniqueTypeList::template Contains<T>(),
                typename UniqueImplement<UniqueTypeList, Ts...>::Type,
                typename UniqueImplement<typename UniqueTypeList::template AppendT<T>, Ts...>::Type
            >;
        };
        template<typename... Ts>
        struct Unique
        {
            using Type = UniqueImplement<TypeList<>, Ts...>::Type;
        };
    }

    /**
     * \brief 类型列表
     */
    template<typename... Types>
    struct TypeList
    {
        /**
         * \brief 列表中类型数量
         */
        static constexpr std::size_t Size = sizeof...(Types);

        /**
         * \brief 是否包含类型T
         */
        template<typename T>
        static constexpr bool Contains() noexcept
        {
            return (... || std::is_same_v<std::remove_cvref_t<T>, std::remove_cvref_t<Types>>);
        }

        /**
         * \brief 是否包含模板T的实例
         */
        template<template<typename...> class T>
        static constexpr bool ContainsInstance() noexcept
        {
            return (... || IsInstanceOf<T, std::remove_cvref_t<Types>>);
        }

        /**
         * \brief 返回类型T在列表中的索引，不存在时返回-1
         */
        template<typename T>
        static constexpr std::int64_t IndexOf() noexcept
        {
            return Detail::IndexOf<T, 0, Types...>();
        }

        /**
         * \brief 返回列表中模板T实例的索引，不存在时返回-1
         */
        template<template<typename...> class T>
        static constexpr std::int64_t IndexOfInstance() noexcept
        {
            return Detail::IndexOfInstance<T, 0, Types...>();
        }

        /**
         * \brief 是否包含类型T的派生类
         */
        template<typename T>
        static constexpr bool ContainsBase() noexcept
        {
            return (... || std::is_base_of_v<std::remove_cvref_t<T>, std::remove_cvref_t<Types>>);
        }

        /**
         * \brief 返回列表中类型T派生类的索引，不存在时返回-1
         */
        template<typename T>
        static constexpr std::int64_t IndexOfBase() noexcept
        {
            return Detail::IndexOfBase<T, 0, Types...>();
        }

        /**
         * \brief 获取列表中第N个类型
         */
        template<std::size_t N>
        static constexpr auto Get() noexcept
        {
            return typename Detail::Get<N, Types...>::Type{};
        }
        template<std::size_t N>
        using GetT = Detail::Get<N, Types...>::Type;

        /**
         * \brief 列表中第一个类型
         */
        static constexpr auto First() noexcept
        {
            return typename Detail::Get<0, Types...>::Type{};
        }
        using FirstT = Detail::Get<0, Types...>::Type;

        /**
         * \brief 列表中最后一个类型
         */
        static constexpr auto Last() noexcept
        {
            return typename Detail::Get<Size - 1, Types...>::Type{};
        }
        using LastT = Detail::Get<Size - 1, Types...>::Type;

        /**
         * \brief 获取除了前N个类型组成的类型列表
         */
        template<std::size_t N>
        static constexpr auto Skip() noexcept
        {
            return typename Detail::Skip<N, Types...>::Type{};
        }
        template<std::size_t N>
        using SkipT = Detail::Skip<N, Types...>::Type;

        /**
         * \brief 获取前N个类型组成的类型列表
         */
        template<std::size_t N>
        static constexpr auto Take() noexcept
        {
            return typename Detail::Take<N, Types...>::Type{};
        }
        template<std::size_t N>
        using TakeT = Detail::Take<N, Types...>::Type;

        /**
         * \brief 拼接类型列表
         */
        template<typename... TypeLists>
        static constexpr auto Concat() noexcept
        {
            return typename Detail::Concat<TypeList, TypeLists...>::Type{};
        }
        template<typename... TypeLists>
        using ConcatT = Detail::Concat<TypeList, TypeLists...>::Type;

        /**
         * \brief 向类型列表后添加类型T
         */
        template<typename T>
        static constexpr auto Append() noexcept
        {
            return typename Detail::Append<T, TypeList>::Type{};
        }
        template<typename T>
        using AppendT = Detail::Append<T, TypeList>::Type;

        /**
         * \brief 向类型列表前添加类型T
         */
        template<typename T>
        static constexpr auto Prepend() noexcept
        {
            return typename Detail::Prepend<T, TypeList>::Type{};
        }
        template<typename T>
        using PrependT = Detail::Prepend<T, TypeList>::Type;

        /**
         * \brief 翻转类型列表
         */
        static constexpr auto Reverse() noexcept
        {
            return typename Detail::Reverse<TypeList>::Type{};
        }
        using ReverseT = Detail::Reverse<TypeList>::Type;

        /**
         * \brief 根据type trait过滤类型列表
         */
        template<template<typename> typename Predicate>
        static constexpr auto FilterByTypeTrait() noexcept
        {
            return typename Detail::FilterByTypeTrait<Predicate, Types...>::Type{};
        }
        template<template<typename> typename Predicate>
        using FilterByTypeTraitT = Detail::FilterByTypeTrait<Predicate, Types...>::Type;

        /**
         * \brief 根据Predicate过滤类型列表
         */
        template<typename F>
        requires (... &&
            (std::is_default_constructible_v<Types> &&
            std::invocable<F, Types> &&
            std::same_as<std::invoke_result_t<F, Types>, bool>))
        static constexpr auto FilterByPredicate(F f)
        {
            return typename Detail::ApplyMask<TypeList, f(Types{})...>::Type{};
        }

        /**
         * \brief 映射类型列表，比如移除元素的引用
         */
        template<template<typename> typename Mapper>
        static constexpr auto Map() noexcept
        {
            return typename Detail::Map<Mapper, Types...>::Type{};
        }
        template<template<typename> typename Mapper>
        using MapT = Detail::Map<Mapper, Types...>::Type;

        /**
         * \brief 无重复类型的列表
         */
        static constexpr auto Unique() noexcept
        {
            return typename Detail::Unique<Types...>::Type{};
        }
        using UniqueT = Detail::Unique<Types...>::Type;
    };
}
