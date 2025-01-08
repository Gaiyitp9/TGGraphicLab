#pragma once

#include <string>

struct Man
{
    int id;
    int age;
    std::string name;
};

struct Any
{
    template<typename T>
    operator T();
};

template<typename T, typename... Args>
consteval auto cnt()
{
    if constexpr (requires {
        T {Args{}..., Any{}};
    })
    {
        return cnt<T, Args..., Any>();
    }
    else
        return sizeof...(Args);
}
