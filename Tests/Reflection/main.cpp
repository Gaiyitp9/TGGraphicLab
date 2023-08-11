#include "CommandLineTest.hpp"
#include "ClangASTTest.hpp"
#include "Reflection/StaticReflection.hpp"
#include "Template/ConstexprString.hpp"

#include <utility>
#include <iostream>

using namespace TG;
using namespace TG::Reflection;

struct Vec
{
    float x;
    float y;
    float norm() const
    {
        return std::sqrt(x * x + y * y);
    }
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
    constexpr static AttributeList<> Attrs;
    constexpr static FieldList Fields = FieldList{
            Field{ConstexprString("x"), &Vec::x},
            Field{ConstexprString("y"), &Vec::y},
            Field{ConstexprString("norm"), &Vec::norm},
    };
};

template<>
struct TG::Reflection::TypeInfo<Color>
{
    constexpr static AttributeList<> Attrs;
    constexpr static FieldList Fields = FieldList{
            Field{ConstexprString("Red"), Color::Red},
            Field{ConstexprString("Green"), Color::Green},
            Field{ConstexprString("Blue"), Color::Blue},
    };
};

int main(int argc, const char **argv)
{
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
//    constexpr std::size_t index = TG::Reflection::TypeInfo<Vec>::Fields.Find(ConstexprString("x"));
//    std::invoke(TG::Reflection::TypeInfo<Vec>::Fields.Get<index>().value, v) = 4.f;
//    std::cout << v.x << std::endl;
//    return 0;
}