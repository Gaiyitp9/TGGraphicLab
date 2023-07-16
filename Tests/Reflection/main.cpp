#include "CommandLineTest.hpp"
#include "ClangASTTest.hpp"
#include "TemplateTest.hpp"
#include "Reflection/StaticReflection.hpp"

#include <utility>
#include <iostream>

template<std::size_t N0, std::size_t... Ns>
void IntList(std::index_sequence<N0, Ns...>)
{
    std::cout << N0 << ' ';
    ((std::cout << Ns << ' '), ...);
    std::cout << std::endl;
}

template<std::size_t... Ns>
void IntList2(std::index_sequence<Ns...>)
{
    ((std::cout << Ns << ' '), ...);
    std::cout << std::endl;
}

int main(int argc, const char **argv)
{
//    return ClangASTTest(argc, argv);
//    TemplateTest();
    IntList2(std::make_index_sequence<0>());
    return 0;
}