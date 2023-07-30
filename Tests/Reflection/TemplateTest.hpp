#pragma once

#include "Template/ConstexprString.hpp"
#include "Reflection/StaticReflection.hpp"
#include <iostream>
#include <functional>

using namespace TG;

struct Vec
{
    float x;
    float y;
};

constexpr int fib(int i)
{
    if (i == 2)
        return i;
    return 0;
}

template<typename T, typename U>
void Test(T, U)
{
    static_assert(std::is_same_v<T, U>);
}

void TemplateTest()
{
    constexpr ConstexprString cstr("what");
    constexpr ConstexprString cstr2("whdt");
//    TStr<"what"> tstr3;
//    Test(tstr, tstr3);

    Vec v{1.0f, 3.0f};
    std::invoke(&Vec::x, v) = 2.0f;
    std::cout << v.x << " " << v.y << std::endl;

    int num;
    std::cin >> num;
    int i = fib(num);
    char str[fib(2)];
    std::cout << i << '\n';
}