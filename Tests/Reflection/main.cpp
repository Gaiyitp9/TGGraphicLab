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
        EXPECT_TRUE(TypeList<>::Size == 0);
        EXPECT_TRUE(TypeList<int>::Size == 1);

        EXPECT_FALSE(TypeList<>::Contains<int>);
        EXPECT_TRUE(TypeList<int>::Contains<int>);
        EXPECT_TRUE((TypeList<int, float>::Contains<int>));

        EXPECT_TRUE((TypeList<int, float>::IndexOf<int> == 0));
        EXPECT_TRUE((TypeList<float, double>::IndexOf<int> == -1));
    }

    TEST(TypeList, BasicOperations)
    {
        EXPECT_TRUE((TypeList<int, float>::Size == 2));
        EXPECT_TRUE((std::is_same_v<TypeList<int>::Get<0>, int>));

        EXPECT_TRUE((std::is_same_v<TypeList<int, float>::First, int>));
        EXPECT_TRUE((std::is_same_v<TypeList<int, float>::Last, float>));

        EXPECT_TRUE((std::is_same_v<TypeList<int, float>::Skip<1>, TypeList<float>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int, float>::Skip<2>, TypeList<>>));

        EXPECT_TRUE((std::is_same_v<TypeList<int, float, double>::Tail, TypeList<float, double>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int, float, double>::Init, TypeList<int, float>>));

        EXPECT_TRUE((std::is_same_v<TypeList<float>::Append<int>, TypeList<float, int>>));
        EXPECT_TRUE((std::is_same_v<TypeList<float>::Prepend<int>, TypeList<int, float>>));
    }

    TEST(TypeList, Concat)
    {
        EXPECT_TRUE((std::is_same_v<TypeList<>::Concat<TypeList<>>, TypeList<>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int, float>::Concat<TypeList<>>, TypeList<int, float>>));
        EXPECT_TRUE((std::is_same_v<TypeList<>::Concat<TypeList<int, float>>, TypeList<int, float>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int, float>::Concat<TypeList<float, int>>,
            TypeList<int, float, float, int>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int, float>::Concat<TypeList<>, TypeList<int, float>>,
            TypeList<int, float, int, float>>));
    }

    TEST(TypeList, Reverse)
    {
        EXPECT_TRUE((std::is_same_v<TypeList<>::Reverse, TypeList<>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int>::Reverse, TypeList<int>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int, float, double>::Reverse, TypeList<double, float, int>>));
    }

    TEST(TypeList, Filter)
    {
        EXPECT_TRUE((std::is_same_v<TypeList<>::Filter<std::is_integral>, TypeList<>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int>::Filter<std::is_integral>, TypeList<int>>));
        EXPECT_TRUE((std::is_same_v<TypeList<float>::Filter<std::is_integral>, TypeList<>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int, float>::Filter<std::is_integral>, TypeList<int>>));
    }

    TEST(TypeList, Map)
    {
        EXPECT_TRUE((std::is_same_v<TypeList<>::Map<std::remove_reference>, TypeList<>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int&>::Map<std::remove_reference>, TypeList<int>>));
        EXPECT_TRUE((std::is_same_v<TypeList<int&, float&>::Map<std::remove_reference>, TypeList<int, float>>));
        EXPECT_FALSE((std::is_same_v<TypeList<float&>::Map<std::remove_reference>, TypeList<int>>));
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
        EXPECT_TRUE((TypeList<int, std::tuple<float>>::ContainsInstance<std::tuple>));
        EXPECT_FALSE((TypeList<int, float>::ContainsInstance<std::tuple>));

        EXPECT_TRUE((TypeList<int, std::tuple<float>>::IndexOfInstance<std::tuple> == 1));
        EXPECT_TRUE((TypeList<int, float>::IndexOfInstance<std::tuple> == -1));
    }


    struct A
    {
        int x;
        void F() {}
    };

    TEST(TypeList, ContainsBase)
    {
        struct B : A{};

        EXPECT_TRUE((TypeList<int, A>::ContainsBase<A>));
        EXPECT_TRUE((TypeList<int, B>::ContainsBase<A>));
        EXPECT_FALSE((TypeList<int, float>::ContainsBase<A>));

        EXPECT_TRUE((TypeList<int, A>::IndexOfBase<A> == 1));
        EXPECT_TRUE((TypeList<int, B>::IndexOfBase<A> == 1));
        EXPECT_TRUE((TypeList<int, float>::IndexOfBase<A> == -1));
    }

    TEST(TypeList, Unique)
    {
        EXPECT_TRUE((std::is_same_v<TypeList<int, float, int>::Unique, TypeList<int, float>>));
    }
}

// int main(int argc, const char **argv)
// {
//     constexpr ConstString str0("test0");
//     constexpr auto str01 = str0.Substr<0, 2>();
//     static_assert(str0[2] == 's');
// //    ConstString str("what");
// //    Test(str);
// //    std::cout << std::boolalpha << std::is_function_v<decltype(Func)> << std::endl;
// //    std::cout << std::boolalpha << std::is_function_v<decltype(SFunc)> << std::endl;
// //    using T = PM_traits<decltype(&ClassTest::Func)>::member_type;
// //    std::cout << std::boolalpha << std::is_member_function_pointer_v<decltype(&ClassTest::Func)> << std::endl;
// //    std::cout << std::boolalpha << std::is_function_v<decltype(&ClassTest::SFunc)> << std::endl;
//
// //    Test(&ClassTest::SFunc);
// //    Test(&ClassTest::Func);
//     constexpr std::string_view str = GetStr();
//     constexpr std::string str2 = GetStr2();
//     static_assert(str.size() == 4);
//     static_assert(str2.size() == 5);
//     constexpr ConstexprTest test;
//     return ClangASTTest(argc, argv);
// //    Vec v{1.0f, 2.0f};
// //    TG::Reflection::TypeInfo<Vec>::Fields.ForEach([](const auto& field) {
// //        std::cout << field.name.View() << std::endl;
// //    });
// //
// //    TG::Reflection::TypeInfo<Color>::Fields.ForEach([](const auto& field) {
// //        std::cout << field.name.View() << std::endl;
// //    });
// //
// //    constexpr std::size_t index = TG::Reflection::TypeInfo<Vec>::Fields.Find(ConstString("x"));
// //    std::invoke(TG::Reflection::TypeInfo<Vec>::Fields.Get<index>().value, v) = 4.f;
// //    std::cout << v.x << std::endl;
// //
// //    std::cout << TG::Reflection::TypeInfo<Vec>::Fields.FindIf(
// //            [&](const auto& field) { return field.name == ConstString("x12"); }) << std::endl;
// //    return 0;
// // }
