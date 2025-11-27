/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "gtest/gtest.h"
#include "Reflection/StaticReflection.hpp"

namespace TG::Reflection
{
    ConstString hello{ "Hello" };

    TEST(ConstString, BasicProperties)
    {
        EXPECT_TRUE(ConstString<0>::NPos == std::string::npos);
        EXPECT_TRUE(hello.Size == 5);
        EXPECT_TRUE(hello.View() == std::string_view("Hello"));
    }

    TEST(ConstString, ComparisonOperators)
    {
        EXPECT_TRUE(hello == ConstString("Hello"));
        EXPECT_TRUE(hello != ConstString("elloH"));
        EXPECT_TRUE(hello != ConstString("ello"));
    }

    constexpr const char* HELLO = "Hello";

    TEST(ConstString, CreateConstStringFromCString)
    {
        constexpr ConstString str = CREATE_CONST_STRING(HELLO);
        EXPECT_TRUE(str.Size == 5);
        EXPECT_TRUE(str == "Hello");
        EXPECT_TRUE(str != "Hell");
    }

    TEST(ConstString, CreateSubstrings)
    {
        EXPECT_TRUE(ConstString().Substr<0>() == "");
        EXPECT_TRUE((ConstString().Substr<0, 0>() == ""));
        EXPECT_TRUE((ConstString().Substr<0, 50>() == ""));
        EXPECT_TRUE((ConstString("Hello").Substr<1>() == "ello"));
        EXPECT_TRUE((ConstString("Hello").Substr<1, 3>() == "ell"));
    }

    TEST(ConstString, Searching)
    {
        EXPECT_TRUE( ConstString("Hello").Find('e') == 1 );
        EXPECT_TRUE( ConstString("Hello").Find('l') == 2 );
        EXPECT_TRUE( ConstString("Hello").Find('w') == static_cast<size_t>(-1) );
        EXPECT_TRUE( ConstString("Hello").Find('l', 3) == 3 );

        EXPECT_TRUE( ConstString("Hello").RFind('e') == 1 );
        EXPECT_TRUE( ConstString("Hello").RFind('l') == 3 );
        EXPECT_TRUE( ConstString("Hello").RFind('w') == static_cast<size_t>(-1) );
        EXPECT_TRUE( ConstString("Hello").RFind('l', 3) == 3 );
    }

    TEST(TypeList, BasicProperties)
    {
        TypeList<> typeList0;
        TypeList<int> typeList1;
        TypeList<int, float> typeList2;
        TypeList<float, double> typeList3;

        EXPECT_TRUE(typeList0.Size == 0);
        EXPECT_TRUE(typeList1.Size == 1);

        EXPECT_FALSE(typeList0.Contains<int>());
        EXPECT_TRUE(typeList1.Contains<int>());
        EXPECT_TRUE(typeList2.Contains<int>());

        EXPECT_TRUE(typeList2.IndexOf<int>() == 0);
        EXPECT_TRUE(typeList3.IndexOf<int>() == -1);
    }

    TEST(TypeList, BasicOperations)
    {
        TypeList<int, float> typeList0;
        EXPECT_TRUE((TypeList<int, float>::Size == 2));
        EXPECT_TRUE((std::is_same_v<decltype(typeList0.Get<0>()), int>));

        TypeList<int> typeList1;
        EXPECT_TRUE((std::is_same_v<TypeList<int>::GetT<0>, int>));
        EXPECT_TRUE((std::is_same_v<decltype(typeList1.Get<0>()), int>));

        EXPECT_TRUE((std::is_same_v<TypeList<int, float>::FirstT, int>));
        EXPECT_TRUE((std::is_same_v<decltype(typeList0.First()), int>));
        EXPECT_TRUE((std::is_same_v<TypeList<int, float>::LastT, float>));
        EXPECT_TRUE((std::is_same_v<decltype(typeList0.Last()), float>));

        EXPECT_TRUE((std::is_same_v<decltype(typeList0.Skip<1>()), TypeList<float>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int, float>::SkipT<1>, TypeList<float>>));
        EXPECT_TRUE((std::is_same_v<decltype(typeList0.Skip<2>()), TypeList<>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int, float>::SkipT<2>, TypeList<>>));

        EXPECT_TRUE((std::is_same_v<decltype(typeList0.Take<1>()), TypeList<int>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int, float>::TakeT<1>, TypeList<int>>));
        EXPECT_TRUE((std::is_same_v<decltype(typeList0.Take<2>()), TypeList<int, float>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int, float>::TakeT<2>, TypeList<int, float>>));

        TypeList<float> typeList2;
        EXPECT_TRUE((std::is_same_v<decltype(typeList2.Append<int>()), TypeList<float, int>>));
        EXPECT_TRUE((std::is_same_v<TypeList<float>::AppendT<int>, TypeList<float, int>>));
        EXPECT_TRUE((std::is_same_v<decltype(typeList2.Prepend<int>()), TypeList<int, float>>));
        EXPECT_TRUE((std::is_same_v<TypeList<float>::PrependT<int>, TypeList<int, float>>));
    }

    TEST(TypeList, Concat)
    {
        TypeList<> typeList0;
        TypeList<int, float> typeList1;
        EXPECT_TRUE((std::is_same_v<decltype(typeList0.Concat<TypeList<>>()), TypeList<>>));
        EXPECT_TRUE((std::is_same_v<TypeList<>::ConcatT<TypeList<>>, TypeList<>>));
        EXPECT_TRUE((std::is_same_v<decltype(typeList1.Concat<TypeList<>>()), TypeList<int, float>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int, float>::ConcatT<TypeList<>>, TypeList<int, float>>));
        EXPECT_TRUE((std::is_same_v<decltype(typeList0.Concat<TypeList<int, float>>()), TypeList<int, float>>));
        EXPECT_TRUE((std::is_same_v<decltype(typeList1.Concat<TypeList<float, int>>()),
            TypeList<int, float, float, int>>));
        EXPECT_TRUE((std::is_same_v<decltype(typeList1.Concat<TypeList<>, TypeList<int, float>>()),
            TypeList<int, float, int, float>>));
    }

    TEST(TypeList, Reverse)
    {
        TypeList<> typeList0;
        TypeList<int> typeList1;
        TypeList<int, float, double> typeList2;
        EXPECT_TRUE((std::is_same_v<decltype(typeList0.Reverse()), TypeList<>>));
        EXPECT_TRUE((std::is_same_v<TypeList<>::ReverseT, TypeList<>>));
        EXPECT_TRUE((std::is_same_v<decltype(typeList1.Reverse()), TypeList<int>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int>::ReverseT, TypeList<int>>));
        EXPECT_TRUE((std::is_same_v<decltype(typeList2.Reverse()), TypeList<double, float, int>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int, float, double>::ReverseT, TypeList<double, float, int>>));
    }

    TEST(TypeList, Filter)
    {
        TypeList<> typeList0;
        TypeList<int> typeList1;
        TypeList<float> typeList2;
        TypeList<int, float> typeList3;
        EXPECT_TRUE((std::is_same_v<decltype(typeList0.FilterByTypeTrait<std::is_integral>()), TypeList<>>));
        EXPECT_TRUE((std::is_same_v<TypeList<>::FilterByTypeTraitT<std::is_integral>, TypeList<>>));
        EXPECT_TRUE((std::is_same_v<decltype(typeList1.FilterByTypeTrait<std::is_integral>()), TypeList<int>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int>::FilterByTypeTraitT<std::is_integral>, TypeList<int>>));
        EXPECT_TRUE((std::is_same_v<decltype(typeList2.FilterByTypeTrait<std::is_integral>()), TypeList<>>));
        EXPECT_TRUE((std::is_same_v<TypeList<float>::FilterByTypeTraitT<std::is_integral>, TypeList<>>));
        EXPECT_TRUE((std::is_same_v<decltype(typeList3.FilterByTypeTrait<std::is_integral>()), TypeList<int>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int, float>::FilterByTypeTraitT<std::is_integral>, TypeList<int>>));

        constexpr auto filteredList0 = TypeList<int, float>::FilterByPredicate(
            []<typename T0>(T0) { return std::is_integral_v<T0>; }
        );
        constexpr auto filteredList1 = typeList3.FilterByPredicate(
            []<typename T0>(T0) { return std::is_integral_v<T0>; }
        );
        EXPECT_TRUE(filteredList0.Size == 1);
        EXPECT_TRUE(filteredList1.Size == 1);
    }

    TEST(TypeList, Map)
    {
        TypeList<> typeList0;
        TypeList<int&> typeList1;
        TypeList<int&, float&> typeList2;
        TypeList<float&> typeList3;
        EXPECT_TRUE((std::is_same_v<decltype(typeList0.Map<std::remove_reference>()), TypeList<>>));
        EXPECT_TRUE((std::is_same_v<TypeList<>::MapT<std::remove_reference>, TypeList<>>));
        EXPECT_TRUE((std::is_same_v<decltype(typeList1.Map<std::remove_reference>()), TypeList<int>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int&>::MapT<std::remove_reference>, TypeList<int>>));
        EXPECT_TRUE((std::is_same_v<decltype(typeList2.Map<std::remove_reference>()), TypeList<int, float>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int&, float&>::MapT<std::remove_reference>, TypeList<int, float>>));
        EXPECT_FALSE((std::is_same_v<decltype(typeList3.Map<std::remove_reference>()), TypeList<int>>));
        EXPECT_FALSE((std::is_same_v<TypeList<float&>::MapT<std::remove_reference>, TypeList<int>>));
    }

    TEST(TypeList, IsInstance)
    {
        EXPECT_TRUE(IsInstance<std::tuple<>>);
        EXPECT_FALSE(IsInstance<int>);

        EXPECT_TRUE((IsInstanceOf<std::tuple, std::tuple<>>));
        EXPECT_FALSE((IsInstanceOf<std::tuple, int>));
        EXPECT_FALSE((IsInstanceOf<std::vector, std::tuple<>>));
    }

    TEST(TypeList, ContainsInstance)
    {
        TypeList<int, std::tuple<float>> typeList0;
        TypeList<int, float> typeList1;
        EXPECT_TRUE(typeList0.ContainsInstance<std::tuple>());
        EXPECT_TRUE((TypeList<int, std::tuple<float>>::ContainsInstance<std::tuple>()));
        EXPECT_FALSE(typeList1.ContainsInstance<std::tuple>());
        EXPECT_FALSE((TypeList<int, float>::ContainsInstance<std::tuple>()));

        EXPECT_TRUE(typeList0.IndexOfInstance<std::tuple>() == 1);
        EXPECT_TRUE((TypeList<int, std::tuple<float>>::IndexOfInstance<std::tuple>() == 1));
        EXPECT_TRUE(typeList1.IndexOfInstance<std::tuple>() == -1);
        EXPECT_TRUE((TypeList<int, float>::IndexOfInstance<std::tuple>() == -1));
    }

    struct A
    {
        int x;
        void F() {}
    };

    struct B : A{};

    TEST(TypeList, ContainsBase)
    {
        TypeList<int, A> typeList0;
        TypeList<int, B> typeList1;
        TypeList<int, float> typeList2;

        EXPECT_TRUE(typeList0.ContainsBase<A>());
        EXPECT_TRUE((TypeList<int, A>::ContainsBase<A>()));
        EXPECT_TRUE(typeList1.ContainsBase<A>());
        EXPECT_TRUE((TypeList<int, B>::ContainsBase<A>()));
        EXPECT_FALSE(typeList2.ContainsBase<A>());
        EXPECT_FALSE((TypeList<int, float>::ContainsBase<A>()));

        EXPECT_TRUE(typeList0.IndexOfBase<A>() == 1);
        EXPECT_TRUE((TypeList<int, A>::IndexOfBase<A>() == 1));
        EXPECT_TRUE(typeList1.IndexOfBase<A>() == 1);
        EXPECT_TRUE((TypeList<int, B>::IndexOfBase<A>() == 1));
        EXPECT_TRUE(typeList2.IndexOfBase<A>() == -1);
        EXPECT_TRUE((TypeList<int, float>::IndexOfBase<A>() == -1));
    }

    TEST(TypeList, Unique)
    {
        TypeList<int, float, int> typeList0;
        EXPECT_TRUE((std::is_same_v<decltype(typeList0.Unique()), TypeList<int, float>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int, float, int>::UniqueT, TypeList<int, float>>));
    }

    TEST(Util, GetInstance)
    {
        std::tuple<Attribute::BaseTypes<A>, Attribute::Property> tuple0{ {}, "test1" };
        auto t0 = Util::GetInstance<Attribute::BaseTypes>(tuple0);
        constexpr std::tuple<Attribute::BaseTypes<A>, Attribute::Property> tuple1{ {}, "test2" };
        constexpr auto t1 = Util::GetInstance<Attribute::BaseTypes>(tuple1);
        EXPECT_TRUE((std::is_same_v<decltype(t0), Attribute::BaseTypes<A>>));
        EXPECT_TRUE((std::is_same_v<std::remove_const_t<decltype(t0.List)>, TypeList<A>>));
        EXPECT_TRUE((std::is_same_v<decltype(t1), const Attribute::BaseTypes<A>>));
        EXPECT_TRUE((std::is_same_v<std::remove_const_t<decltype(t1.List)>, TypeList<A>>));
    }

    struct C : B{};

    template<>
    struct TypeInfo<A>
    {
        using Type = A;

        static constexpr ConstString Name{ "A" };
        static constexpr auto Attributes{ Attribute::MakeAttributes<Attribute::Usage::Type>() };
        static constexpr std::size_t MemberCount{ 1 };

        template<std::size_t, typename = void>
        struct Member
        {

        };

        template<typename Unused>
        struct Member<0, Unused>
        {
            static constexpr ConstString Name{ "F" };
            static constexpr auto Accessibility{ AccessSpecifier::Public };
            static constexpr auto MemberType{ MemberType::Function };
            static constexpr auto Attributes{ Attribute::MakeAttributes<Attribute::Usage::Function>() };

            template<typename Self, typename... Args>
            static constexpr auto Invoke(Self&& self, Args&&... args)
                -> decltype(Detail::ForwardCast<Self, Type>(self).F(std::forward<Args>(args)...))
            {
                return Detail::ForwardCast<Self, Type>(self).F(std::forward<Args>(args)...);
            }

            static constexpr auto Pointer() requires requires { &Type::F; }
            {
                return &Type::F;
            }

            template<typename Pointer>
            static constexpr auto Resolve() requires requires { static_cast<Pointer>(&Type::F); }
            {
                return static_cast<Pointer>(&Type::F);
            }
        };
    };

    template<>
    struct TypeInfo<B>
    {
        using Type = B;

        static constexpr ConstString Name{ "B" };
        static constexpr auto Attributes{ Attribute::MakeAttributes<Attribute::Usage::Type>(Attribute::BaseTypes<A>{}) };
        static constexpr std::size_t MemberCount{ 0 };

    };

    template<>
    struct TypeInfo<C>
    {
        using Type = C;

        static constexpr ConstString Name{ "C" };
        static constexpr auto Attributes{ Attribute::MakeAttributes<Attribute::Usage::Type>(Attribute::BaseTypes<B>{}) };
        static constexpr std::size_t MemberCount{ 0 };
    };

    TEST(TypeDescriptor, BasicProperties)
    {
        TypeDescriptor<A> t0;
        TypeDescriptor<B> t1;
        TypeDescriptor<C> t2;
        EXPECT_TRUE(t0.Name == "A");
        EXPECT_TRUE(t0.DeclaredBases.Size == 0);
        EXPECT_TRUE(t0.Bases.Size == 0);
        EXPECT_TRUE(t0.DeclaredMembers.Size == 1);
        EXPECT_TRUE(t0.Members.Size == 1);
        EXPECT_TRUE(t0.DeclaredMembers.Get<0>().Name == "F");
        EXPECT_TRUE(t1.Name == "B");
        EXPECT_TRUE(t1.DeclaredBases.Size == 1);
        EXPECT_TRUE(t1.Bases.Size == 1);
        EXPECT_TRUE(t1.DeclaredMembers.Size == 0);
        EXPECT_TRUE(t1.Members.Size == 1);
        EXPECT_TRUE(t2.Name == "C");
        EXPECT_TRUE(t2.DeclaredBases.Size == 1);
        EXPECT_TRUE(t2.Bases.Size == 2);
        EXPECT_TRUE(t2.DeclaredMembers.Size == 0);
        EXPECT_TRUE(t2.Members.Size == 1);
    }
}
