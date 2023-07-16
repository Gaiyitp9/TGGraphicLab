#include <type_traits>
#include <iostream>
#include "Template/ConstexprString.hpp"

template<typename T>
void Test(T& t)
{
    std::cout << "T&: " << __FUNCSIG__ << std::endl;
}

template<typename T>
void Test1(const T& t)
{
    std::cout << "const T&: " << __FUNCSIG__ << std::endl;
}

template<typename T>
void Test2(T t)
{
    std::cout << "T: " << __FUNCSIG__ << std::endl;
}

template<typename T>
void Test3(const T t)
{
    std::cout << "const T: " << __FUNCSIG__ << std::endl;
}

template<typename Text = void>
struct ModifierTest
{
    virtual void Print()
    {
        std::cout << "general one" << std::endl;
    }
};
template<typename Text>
struct ModifierTest<const Text> : ModifierTest<Text>
{
    void Print() override
    {
        std::cout << "const one" << std::endl;
        ModifierTest<Text>::Print();
    }
};
template<typename Text>
struct ModifierTest<Text*> : ModifierTest<Text>
{
    void Print() override
    {
        std::cout << "pointer one" << std::endl;
        ModifierTest<Text>::Print();
    }
};

void AutoTest(auto a, auto b)
{
    std::cout << "AutoTest: " << __FUNCSIG__ << std::endl;
    std::cout << a << " " << b << std::endl;
}

template<int... nums>
void VariableParameter()
{
    (std::cout << ... << nums);
    std::cout << std::endl;
}

// T有以Nums...中所有类型作为函数参数的SpecificFunc函数
template<typename T, typename... Nums>
concept HasSpecificFunc = requires(T t)
{
    (t.template SpecificFunc(std::declval<Nums>()), ...);
};
// 一个不包含默认构造函数的类，用于验证declval不要求有默认构造函数
struct NonDefaultConstructorObj
{
    explicit NonDefaultConstructorObj(int i) : num(i) {}

    int num;
};
struct NonDefaultConstructorObj2
{
    explicit NonDefaultConstructorObj2(int i) : num(i) {}

    int num;
};
// 符合HasSpecificFunc概念的类
class SpecificFuncTest
{
public:
    template<typename T>
    void SpecificFunc(T i) { std::cout << i.num << std::endl; }
};
// 测试HasSpecificFunc概念的函数
template<typename T, typename... Nums> requires HasSpecificFunc<T, Nums...>
void HasSpecificFuncTest(T t, Nums... nums)
{
    (t.template SpecificFunc(nums), ...);
}

int main()
{
    // [const] [volatile] <type> [*|&] [[N]]碰到T，会退化(decay)为 <type> [*](当类型为指针或数组时)
    // [const] [volatile] <type> [*|&] [[N]]碰到T&，不变
    // 注：[]内表示可能出现的词，<>内表示一定要出现的词
    // 下面是几个简单的示例
    const int i = 2;
    char const* str = "what";
    char const str2[5] = "what";
    const char& ch = 's';
    // T&
    Test(i);        // const int -> const int
    Test(str);      // char const* -> char const*
    Test(str2);     // char const[N] -> char const[N]
    Test(ch);       // const char& -> const char
    // const T&
    Test1(i);        // const int -> int
    Test1(str);      // char const* -> char const*
    Test1(str2);     // char const[N] -> char[N]
    Test1(ch);       // const char& -> char
    // T
    Test2(i);       // const int -> int
    Test2(str);     // char const* -> char const*
    Test2(str2);    // char const[N] -> char const*
    Test2(ch);      // const char& -> char
    // const T
    Test3(i);       // const int -> int
    Test3(str);     // char const* -> char const*
    Test3(str2);    // char const[N] -> char const*
    Test3(ch);      // const char& -> char

    //-----------------------------------------------------------------------------------------------

    // std::remove_const不能移除const char*的const修饰符，可以移除const char[N]的const修饰符
    // 因为const char*表示指向const char的指针，实际上该指针并不是const，所以也就谈不上移除const修饰符
    // 而const char[N]表示const的数组类型，所以可以移除const修饰符
    static_assert(std::is_same_v<std::remove_const_t<char const*>, char const*>);
    static_assert(std::is_same_v<std::remove_const_t<const char[5]>, char[5]>);

    //-----------------------------------------------------------------------------------------------

    // 这里说明：const char*会优先匹配指针特化类型，再匹配const特化类型
    ModifierTest<const char*> trait;
    trait.Print();

    //-----------------------------------------------------------------------------------------------

    std::string_view strv("what?");
    constexpr TG::ConstexprString cstr("what");

    //-----------------------------------------------------------------------------------------------
    // 测试auto作为函数参数
    AutoTest(2, "jjs");

    //-----------------------------------------------------------------------------------------------
    // 测试折叠表达式
    VariableParameter<2, 3, 4, 5, 7>();

    //-----------------------------------------------------------------------------------------------
    // 测试折叠表达式和std::declval
    SpecificFuncTest s;
    NonDefaultConstructorObj obj0(3);
    NonDefaultConstructorObj obj1(4);
    NonDefaultConstructorObj2 obj2(5);
    HasSpecificFuncTest(s, obj0, obj1, obj2);

    return 0;
}
