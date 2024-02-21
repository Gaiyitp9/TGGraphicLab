#include "CommandLineTest.hpp"
#include "ClangASTTest.hpp"
#include "Reflection/StaticReflection.hpp"
#include "ConstString.hpp"
#include "Diagnostics/Log.hpp"

#include <utility>
#include <iostream>

using namespace TG;
using namespace TG::Reflection;

struct Vec
{
    float x;
    float y;
    float Norm() const
    {
        return std::sqrt(x * x + y * y);
    }

    constexpr static int Id = -1;
};

enum class Color
{
    Red,
    Green,
    Blue
};

template<>
struct TG::Reflection::TypeInfo<Vec>
{
    constexpr static AttributeList Attrs = AttributeList{};
    constexpr static FieldList Fields = FieldList{
            Field{ConstString("x"), &Vec::x},
            Field{ConstString("y"), &Vec::y},
            Field{ConstString("Norm"), &Vec::Norm},
            Field{ConstString("id"), &Vec::Id},
    };
};

template<>
struct TG::Reflection::TypeInfo<Color>
{
    constexpr static AttributeList Attrs = AttributeList{};
    constexpr static FieldList Fields = FieldList{
            Field{ConstString("Red"), Color::Red},
            Field{ConstString("Green"), Color::Green},
            Field{ConstString("Blue"), Color::Blue},
    };
};

template<typename T>
void Test(T)
{
}

void Func()
{

}

static void SFunc()
{

}

class ClassTest
{
public:
    void Func() {}
    static void SFunc() {}
};

template<typename>
struct PM_traits {};

template<class T, class U>
struct PM_traits<U T::*> { using member_type = U; };

constexpr std::string_view GetStr()
{
    return "jsjs";
}

constexpr std::string GetStr2()
{
    constexpr std::string temp("jsjss");
    return temp;
}

struct ConstexprTest
{
    constexpr ConstexprTest() : mVec("wsssssssaaaaash") {}
    std::string mVec;
};

int main(int argc, const char **argv)
{
    constexpr ConstString str0("test0");
    constexpr auto str01 = str0.Substr<0, 2>();
    static_assert(str0[2] == 's');
//    ConstString str("what");
//    Test(str);
//    std::cout << std::boolalpha << std::is_function_v<decltype(Func)> << std::endl;
//    std::cout << std::boolalpha << std::is_function_v<decltype(SFunc)> << std::endl;
//    using T = PM_traits<decltype(&ClassTest::Func)>::member_type;
//    std::cout << std::boolalpha << std::is_member_function_pointer_v<decltype(&ClassTest::Func)> << std::endl;
//    std::cout << std::boolalpha << std::is_function_v<decltype(&ClassTest::SFunc)> << std::endl;

//    Test(&ClassTest::SFunc);
//    Test(&ClassTest::Func);
    constexpr std::string_view str = GetStr();
    constexpr std::string str2 = GetStr2();
    static_assert(str.size() == 4);
    static_assert(str2.size() == 5);
    constexpr ConstexprTest test;
    return ClangASTTest(argc, argv);
//    Vec v{1.0f, 2.0f};
//    TG::Reflection::TypeInfo<Vec>::Fields.ForEach([](const auto& field) {
//        std::cout << field.name.View() << std::endl;
//    });
//
//    TG::Reflection::TypeInfo<Color>::Fields.ForEach([](const auto& field) {
//        std::cout << field.name.View() << std::endl;
//    });
//
//    constexpr std::size_t index = TG::Reflection::TypeInfo<Vec>::Fields.Find(ConstString("x"));
//    std::invoke(TG::Reflection::TypeInfo<Vec>::Fields.Get<index>().value, v) = 4.f;
//    std::cout << v.x << std::endl;
//
//    std::cout << TG::Reflection::TypeInfo<Vec>::Fields.FindIf(
//            [&](const auto& field) { return field.name == ConstString("x12"); }) << std::endl;
//    return 0;
}