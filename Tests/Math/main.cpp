// #include "Math/Core.hpp"
// #include <iostream>
//
// int main()
// {
//     TG::Math::Matrix4f v1, v2, v3;
//     v1(0, 0) = v1(1, 1) = v1(2, 2) = v1(3, 3) = 1;
//     v2(0, 0) = v2(1, 1) = v2(2, 2) = v2(3, 3) = 3;
//     v3(0, 0) = v3(1, 1) = v3(2, 2) = v3(3, 3) = 2;
//     TG::Math::Matrix4f a = v1.CWiseProduct(v2);
//     TG::Math::Matrix4f b = v1 + v2 - v3;
//     b(2, 3) = -1;
//     b(1, 3) = -1;
//     TG::Math::Matrix3f c;
//     c = b.Block<3, 3>(1, 1);
//     TG::Math::Matrix4f d = b.Transpose();
//
//     std::cout << "sum of a: " << a.Sum() << '\n';
//     std::cout << "sum of a block: " << b.Block<3, 3>(1, 1).Sum() << '\n';
//     for (int i = 0; i < 4; ++i)
//     {
//         for (int j = 0; j < 4; ++j)
//         {
//             std::cout << d(i, j) << ' ';
//         }
//         std::cout << '\n';
//     }
//     for (int i = 0; i < 3; ++i)
//     {
//         for (int j = 0; j < 3; ++j)
//         {
//             std::cout << c(i, j) << ' ';
//         }
//         std::cout << '\n';
//     }
//     return 0;
// }
#include <format>
#include <iostream>
#include <iterator>
#include <memory_resource>
#include <string>

std::pmr::string PMRStr(std::pmr::monotonic_buffer_resource *mbr)
{
    std::pmr::string buffer{mbr};

    std::format_to(
        std::back_inserter(buffer), //< OutputIt
        "Hello, C++{}!\n",          //< fmt
        "20");                      //< arg
    std::cout << buffer;
    return buffer;
}

#include <type_traits>
#include <memory>
#include <forward_list>

std::unique_ptr<int> TestUnique()
{
    std::unique_ptr<int> pp = std::make_unique<int>();
    *pp = 3;
    return pp;
}

struct base
{
    virtual void print() { std::cout << "base class" << std::endl; }
};

struct derived : base
{
    void print() override { std::cout << "derived class" << num << std::endl; }
    int num = 0;
};

struct derived1 : base
{
    void print() override { std::cout << "derived1 class" << num << std::endl; }
    int num = 0;
};

struct Base
{
    // virtual void Print() = 0;
    virtual ~Base() { std::cout << "base" << std::endl; };
};
struct Derived : Base
{
    // void Print() override { std::cout << "Derived\n"; }
    int i{ 2 };
};
struct Derived1 : Base
{
    // ~Derived1() override { std::cout << "derived1" << std::endl; }
    // void Print() override { std::cout << "Derived1\n"; }
    int j{ 3 };
};
void TypeIdTest(const Base& b)
{
    std::cout << std::boolalpha << (typeid(b) != typeid(Derived1)) << std::endl;
}

int main()
{
    Derived1 d;
    TypeIdTest(d);

    int* inum = new int(2);
    std::reference_wrapper<int> iwrap(*inum);
    delete inum;
    // std::unique_ptr<int> p2;
    // p2 = TestUnique();
    // std::pmr::monotonic_buffer_resource mbr;
    // std::pmr::string buffer1 = PMRStr(&mbr);;
    // std::cout << buffer1;
    // // buffer.clear();
    //
    // derived d;
    // d.num = 2;
    // derived1 d1;
    // const derived1& d2 = d1;
    // std::vector<std::reference_wrapper<base>> vb;
    // vb.emplace_back(d);
    // vb.emplace_back(d1);
    // for (auto b : vb)
    //     b.get().print();
    // std::erase_if(vb, [&d2](const std::reference_wrapper<base> b) { return std::addressof(b.get()) == std::addressof(d2); });
    // for (auto b : vb)
    //     b.get().print();
    // std::format_to(
    //     std::back_inserter(buffer), //< OutputIt
    //     "Hello, {0}::{1}!{2}",      //< fmt
    //     "std",                      //< arg {0}
    //     "format_to()",              //< arg {1}
    //     "\n",                       //< arg {2}
    //     "extra param(s)...");       //< unused
    // std::cout << buffer;
    //
    // std::wstring wbuffer;
    // std::format_to(
    //     std::back_inserter(wbuffer),//< OutputIt
    //     L"Hello, {2}::{1}!{0}",     //< fmt
    //     L"\n",                      //< arg {0}
    //     L"format_to()",             //< arg {1}
    //     L"std",                     //< arg {2}
    //     L"...is not..."             //< unused
    //     L"...an error!");           //< unused
    // std::wcout << wbuffer;
}