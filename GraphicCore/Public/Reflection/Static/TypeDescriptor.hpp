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
        template<typename T>
        using AttributeTypes = AsTypeList<std::remove_cv_t<decltype(TypeInfo<T>::Attributes)>>::Type;

        template<typename>
        struct Flatten;
        template<typename... TypeLists>
        struct Flatten<TypeList<TypeLists...>>
        {
            using Type = Concat<TypeLists...>::Type;
        };

        template<typename T, bool HasBases>
        struct EnumerateDeclaredBaseTypeList;
        template<typename T>
        struct EnumerateDeclaredBaseTypeList<T, false>
        {
            using Type = TypeList<>;
        };
        template<typename T>
        struct EnumerateDeclaredBaseTypeList<T, true>
        {
            using Type = std::remove_cvref_t<
                    decltype(Util::GetInstance<Attribute::BaseTypes>(TypeInfo<T>::Attributes))
                >::ListType;
        };

        template<typename T>
        struct DeclaredBaseTypeList
        {
            using Type = EnumerateDeclaredBaseTypeList<
                T,
                AttributeTypes<T>::template ContainsInstance<Attribute::BaseTypes>()
            >::Type;
        };

        template<typename T>
        struct BaseTypeList
        {
            using DeclaredBases = DeclaredBaseTypeList<T>::Type;
            using RecursiveBases = Flatten<typename DeclaredBases::template MapT<BaseTypeList>>::Type;

            using Type = DeclaredBases::template ConcatT<RecursiveBases>::UniqueT;
        };

        template<typename T, std::size_t N>
        using MakeDescriptor = std::conditional_t<
            IsField<MemberInfo<T, N>>,
            FieldDescriptor<T, N>,
            std::conditional_t<
                IsFunction<MemberInfo<T, N>>,
                FunctionDescriptor<T, N>,
                void
            >
        >;
        template<typename T, typename IndexSequence>
        struct EnumerateMembers;
        template<typename T>
        struct EnumerateMembers<T, std::index_sequence<>>
        {
            using Type = TypeList<>;
        };
        template<typename T, std::size_t... Indices>
        struct EnumerateMembers<T, std::index_sequence<Indices...>>
        {
            using Type = TypeList<MakeDescriptor<T, Indices>...>;
        };

        template<typename T>
        struct DeclaredMemberList
        {
            using Type = EnumerateMembers<T, std::make_index_sequence<TypeInfo<T>::MemberCount>>::Type;
        };

        template<typename T>
        struct MemberList
        {
            using Type = Flatten<
                typename BaseTypeList<T>::Type::template PrependT<T>::template MapT<DeclaredMemberList>
            >::Type;
        };
    }

    /**
     * \brief 判断类型T是否是类型描述符
     */
    template<typename T>
    concept IsType = IsInstanceOf<TypeDescriptor, T>;

    /**
     * \brief 判断类型T是否是描述符
     */
    template<typename T>
    concept IsDescriptor = IsType<T> || IsMember<T>;

    template<typename T>
    struct TypeDescriptor
    {
        using Type = T;

        using DeclaredBaseTypes = Detail::DeclaredBaseTypeList<T>::Type;

        using BaseTypes = Detail::BaseTypeList<T>::Type;

        using DeclaredMemberTypes = Detail::DeclaredMemberList<T>::Type;

        using MemberTypes = Detail::MemberList<T>::Type;

        using AttributeTypes = Detail::AttributeTypes<T>;

        static constexpr auto Name{ TypeInfo<T>::Name };

        static constexpr DeclaredBaseTypes DeclaredBases{};

        static constexpr BaseTypes Bases{};

        static constexpr DeclaredMemberTypes DeclaredMembers{};

        static constexpr MemberTypes Members{};

        static constexpr auto Attributes{ TypeInfo<T>::Attributes };
    };
}
