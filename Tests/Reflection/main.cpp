#include "CommandLineTest.hpp"
#include "ClangASTTest.hpp"
#include "TemplateTest.hpp"
#include "Reflection/StaticReflection.hpp"

#include <utility>
#include <iostream>

using namespace TG::Reflection;

struct Vector
{
    float x;
    float y;
    float norm() const
    {
        return std::sqrt(x * x + y * y);
    }
};

template<>
struct TG::Reflection::TypeInfo<Vector>
{
    constexpr static AttributeList<> Attrs;
    constexpr static FieldList Fields = FieldList{
            Field{ConstexprString("x"), &Vector::x},
            Field{ConstexprString("y"), &Vector::y},
            Field{ConstexprString("norm"), &Vector::norm},
    };
};

template<std::size_t M>
struct Test1
{
//    template<std::size_t N>
//    constexpr operator==(Test)
};

int main(int argc, const char **argv)
{
//    return ClangASTTest(argc, argv);
//    TemplateTest();

    Vector v{1.0f, 2.0f};
    TG::Reflection::TypeInfo<Vector>::Fields.ForEach([](const auto& field) {
        std::cout << field.name.View() << std::endl;
    });

    constexpr std::size_t index = TG::Reflection::TypeInfo<Vector>::Fields.Find(ConstexprString("x"));
    std::invoke(TG::Reflection::TypeInfo<Vector>::Fields.Get<index>().value, v) = 4.f;
    std::cout << v.x << std::endl;
    return 0;
}