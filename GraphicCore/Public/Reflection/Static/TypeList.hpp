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

        template<template<typename> class Predicate, typename... Ts>
        struct Filter;
        template<template<typename> class Predicate>
        struct Filter<Predicate>
        {
            using Type = TypeList<>;
        };
        template<template<typename> class Predicate, typename T, typename... Ts>
        struct Filter<Predicate, T, Ts...>
        {
            using Type = std::conditional_t<Predicate<T>::value,
                typename Prepend<T, typename Filter<Predicate, Ts...>::Type>::Type,
                typename Filter<Predicate, Ts...>::Type
            >;
        };

        template<template<typename> class Mapper, typename... Ts>
        struct Map;
        template<template<typename> class Mapper>
        struct Map<Mapper>
        {
            using Type = TypeList<>;
        };
        template<template<typename> class Mapper, typename T, typename... Ts>
        struct Map<Mapper, T, Ts...>
        {
            using Type = Prepend<typename Mapper<T>::type, typename Map<Mapper, Ts...>::Type>::Type;
        };

        template<typename T, std::int64_t N, typename... Ts>
        constexpr std::int64_t IndexOf() noexcept
        {
            if constexpr (sizeof...(Ts) <= N)
                return -1;
            else if constexpr (std::is_same_v<T, typename Get<N, Ts...>::Type>)
                return N;
            else
                return IndexOf<T, N + 1, Ts...>();
        }

        template<template<typename...> class T, std::int64_t N, typename... Ts>
        constexpr std::int64_t IndexOfInstance() noexcept
        {
            if constexpr (sizeof...(Ts) <= N)
                return -1;
            else if constexpr (Reflection::IsInstanceOf<T, typename Get<N, Ts...>::Type>)
                return N;
            else
                return IndexOfInstance<T, N + 1, Ts...>();
        }

        template<typename T, std::int64_t N, typename... Ts>
        constexpr std::int64_t IndexOfBase() noexcept
        {
            if constexpr (sizeof...(Ts) <= N)
                return -1;
            else if constexpr (std::is_base_of_v<T, typename Get<N, Ts...>::Type>)
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
                UniqueTypeList::template Contains<T>,
                typename UniqueImplement<UniqueTypeList, Ts...>::Type,
                typename UniqueImplement<typename UniqueTypeList::template Append<T>, Ts...>::Type
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
        static constexpr bool Contains =
            (... || std::is_same_v<std::remove_cvref_t<T>, std::remove_cvref_t<Types>>);
        /**
         * \brief 是否包含模板T的实例
         */
        template<template<typename...> class T>
        static constexpr bool ContainsInstance = (... || IsInstanceOf<T, std::remove_cvref_t<Types>>);
        /**
         * \brief 返回类型T在列表中的索引，不存在时返回-1
         */
        template<typename T>
        static constexpr std::int64_t IndexOf = Detail::IndexOf<T, 0, Types...>();
        /**
         * \brief 返回列表中模板T实例的索引，不存在时返回-1
         */
        template<template<typename...> class T>
        static constexpr std::int64_t IndexOfInstance = Detail::IndexOfInstance<T, 0, Types...>();
        /**
         * \brief 是否包含类型T的派生类
         */
        template<typename T>
        static constexpr bool ContainsBase =
            (... || std::is_base_of_v<std::remove_cvref_t<T>, std::remove_cvref_t<Types>>);
        /**
         * \brief 返回列表中类型T派生类的索引，不存在时返回-1
         */
        template<typename T>
        static constexpr std::int64_t IndexOfBase = Detail::IndexOfBase<T, 0, Types...>();
        /**
         * \brief 获取列表中第N个类型
         */
        template<std::size_t N>
        using Get = Detail::Get<N, Types...>::Type;
        /**
         * \brief 列表中第一个类型
         */
        using First = Get<0>;
        /**
         * \brief 列表中最后一个类型
         */
        using Last = Get<Size - 1>;
        /**
         * \brief 获取除了前N个类型组成的类型列表
         */
        template<std::size_t N>
        using Skip = Detail::Skip<N, Types...>::Type;
        using Tail = Skip<1>;
        /**
         * \brief 获取前N个类型组成的类型列表
         */
        template<std::size_t N>
        using Take = Detail::Take<N, Types...>::Type;
        using Init = Take<Size - 1>;
        /**
         * \brief 拼接类型列表
         */
        template<typename... TypeLists>
        using Concat = Detail::Concat<TypeList, TypeLists...>::Type;
        /**
         * \brief 向类型列表后添加类型T
         */
        template<typename T>
        using Append = Detail::Append<T, TypeList>::Type;
        /**
         * \brief 向类型列表前添加类型T
         */
        template<typename T>
        using Prepend = Detail::Prepend<T, TypeList>::Type;
        /**
         * \brief 翻转类型列表
         */
        using Reverse = Detail::Reverse<TypeList>::Type;
        /**
         * \brief 筛选类型列表
         */
        template<template<typename> class Predicate>
        using Filter = Detail::Filter<Predicate, Types...>::Type;
        /**
         * \brief 映射类型列表，比如移除元素的引用
         */
        template<template<typename> class Mapper>
        using Map = Detail::Map<Mapper, Types...>::Type;
        /**
         * \brief 无重复类型的列表
         */
        using Unique = Detail::Unique<Types...>::Type;
    };
}
