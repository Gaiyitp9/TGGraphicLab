#include "gtest/gtest.h"
#include "Math/Core.hpp"
#include <random>

namespace TG::Math
{
    static float gEpsilon = 0.001f;
    static float gMin = gMin;
    static float gMax = 100.0f;
    
    TEST(TestMatrix, Constructor)
    {
        Matrix4f mat;
        for (int i = 0; i < 16; ++i)
            EXPECT_NEAR(mat[i], 0, gEpsilon);
    }

    TEST(TestMatrix, CWiseBinaryOp)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution urd(gMin, gMax);

        Matrix4f mat1, mat2;
        for (int i = 0; i < 16; ++i)
        {
            mat1[i] = urd(gen);
            mat2[i] = urd(gen);
        }
        Matrix4f result = mat1 + mat2;
        for (int i = 0; i < 16; ++i)
            EXPECT_NEAR(result[i], mat1[i] + mat2[i], gEpsilon);

        result = mat1 - mat2;
        for (int i = 0; i < 16; ++i)
            EXPECT_NEAR(result[i], mat1[i] - mat2[i], gEpsilon);

        result = mat1.CWiseProduct(mat2);
        for (int i = 0; i < 16; ++i)
            EXPECT_NEAR(result[i], mat1[i] * mat2[i], gEpsilon);
    }

    TEST(TestMatrix, Block)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution urd(gMin, gMax);

        Matrix4f mat4;
        for (int i = 0; i < 16; ++i)
            mat4[i] = urd(gen);

        Matrix3f mat3 = mat4.block<1, 1, 3, 3>();
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                EXPECT_NEAR(mat3(i, j), mat4(i + 1, j + 1), gEpsilon);

        Matrix2f mat2;
        for (int i = 0; i < 4; ++i)
            mat2[i] = urd(gen);
        mat4.block<0, 0, 2, 2>() = mat2;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                EXPECT_NEAR(mat4(i, j), mat2(i, j), gEpsilon);
    }

    TEST(TestMatrix, Transpose)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution urd(gMin, gMax);

        Matrix4f mat4;
        for (int i = 0; i < 16; ++i)
            mat4[i] = urd(gen);

        Matrix4f transpose = mat4.transpose();
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                EXPECT_NEAR(transpose(i, j), mat4(j, i), gEpsilon);

        Matrix4f transpose1 = mat4.transpose().transpose();
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                EXPECT_NEAR(transpose1(i, j), mat4(i, j), gEpsilon);
    }

    TEST(TestMatrix, Reduce)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution urd(gMin, gMax);

        Matrix4f mat4;
        for (int i = 0; i < 16; ++i)
            mat4[i] = urd(gen);

        float temp = mat4.Sum();
        float sum = 0.0f;
        for (int i = 0; i < 16; ++i)
            sum += mat4[i];
        EXPECT_NEAR(sum, temp, gEpsilon);
    }

    TEST(TestMatrix, Dot)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution urd(gMin, gMax);

        Vector4f vec0, vec1;
        for (int i = 0; i < 4; ++i)
        {
            vec0[i] = urd(gen);
            vec1[i] = urd(gen);
        }

        float temp = vec0.Dot(vec1);
        float dot = 0;
        for (int i = 0; i < 4; ++i)
            dot += vec0[i] * vec1[i];
        EXPECT_NEAR(dot, temp, gEpsilon);
    }
}

// #include <format>
// #include <iostream>
// #include <iterator>
// #include <memory_resource>
// #include <string>
//
// std::pmr::string PMRStr(std::pmr::monotonic_buffer_resource *mbr)
// {
//     std::pmr::string buffer{mbr};
//
//     std::format_to(
//         std::back_inserter(buffer), //< OutputIt
//         "Hello, C++{}!\n",          //< fmt
//         "20");                      //< arg
//     std::cout << buffer;
//     return buffer;
// }
//
// #include <type_traits>
// #include <memory>
// #include <forward_list>
//
// std::unique_ptr<int> TestUnique()
// {
//     std::unique_ptr<int> pp = std::make_unique<int>();
//     *pp = 3;
//     return pp;
// }
//
// struct base
// {
//     virtual void print() { std::cout << "base class" << std::endl; }
// };
//
// struct derived : base
// {
//     void print() override { std::cout << "derived class" << num << std::endl; }
//     int num = 0;
// };
//
// struct derived1 : base
// {
//     void print() override { std::cout << "derived1 class" << num << std::endl; }
//     int num = 0;
// };
//
// struct Base
// {
//     // virtual void Print() = 0;
//     virtual ~Base() { std::cout << "base" << std::endl; };
// };
// struct Derived : Base
// {
//     // void Print() override { std::cout << "Derived\n"; }
//     int i{ 2 };
// };
// struct Derived1 : Base
// {
//     // ~Derived1() override { std::cout << "derived1" << std::endl; }
//     // void Print() override { std::cout << "Derived1\n"; }
//     int j{ 3 };
// };
// void TypeIdTest(const Base& b)
// {
//     std::cout << std::boolalpha << (typeid(b) != typeid(Derived1)) << std::endl;
// }
//
// int main()
// {
//     Derived1 d;
//     TypeIdTest(d);
//
//     int* inum = new int(2);
//     std::reference_wrapper<int> iwrap(*inum);
//     delete inum;
//     // std::unique_ptr<int> p2;
//     // p2 = TestUnique();
//     // std::pmr::monotonic_buffer_resource mbr;
//     // std::pmr::string buffer1 = PMRStr(&mbr);;
//     // std::cout << buffer1;
//     // // buffer.clear();
//     //
//     // derived d;
//     // d.num = 2;
//     // derived1 d1;
//     // const derived1& d2 = d1;
//     // std::vector<std::reference_wrapper<base>> vb;
//     // vb.emplace_back(d);
//     // vb.emplace_back(d1);
//     // for (auto b : vb)
//     //     b.get().print();
//     // std::erase_if(vb, [&d2](const std::reference_wrapper<base> b) { return std::addressof(b.get()) == std::addressof(d2); });
//     // for (auto b : vb)
//     //     b.get().print();
//     // std::format_to(
//     //     std::back_inserter(buffer), //< OutputIt
//     //     "Hello, {0}::{1}!{2}",      //< fmt
//     //     "std",                      //< arg {0}
//     //     "format_to()",              //< arg {1}
//     //     "\n",                       //< arg {2}
//     //     "extra param(s)...");       //< unused
//     // std::cout << buffer;
//     //
//     // std::wstring wbuffer;
//     // std::format_to(
//     //     std::back_inserter(wbuffer),//< OutputIt
//     //     L"Hello, {2}::{1}!{0}",     //< fmt
//     //     L"\n",                      //< arg {0}
//     //     L"format_to()",             //< arg {1}
//     //     L"std",                     //< arg {2}
//     //     L"...is not..."             //< unused
//     //     L"...an error!");           //< unused
//     // std::wcout << wbuffer;
// }
