#include "gtest/gtest.h"
#include "Math/Core.hpp"
#include <random>

namespace TG::Math
{
    static float gEpsilon = 0.001f;
    static float gMin = 0.1f;
    static float gMax = 100.0f;

    static std::random_device gRd;
    static std::mt19937 gGen(gRd());
    static std::uniform_real_distribution gUrd(gMin, gMax);
    
    TEST(TestMatrix, Constructor)
    {
        Matrix4F mat;
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(mat[i], 0, gEpsilon);
    }

    TEST(TestMatrix, CWiseBinaryOp)
    {
        Matrix4F mat1, mat2;
        for (std::size_t i = 0; i < 16; ++i)
        {
            mat1[i] = gUrd(gGen);
            mat2[i] = gUrd(gGen);
        }
        Matrix4F result = mat1 + mat2;
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(result[i], mat1[i] + mat2[i], gEpsilon);

        result = mat1 - mat2;
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(result[i], mat1[i] - mat2[i], gEpsilon);

        result = mat1.CWiseProduct(mat2);
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(result[i], mat1[i] * mat2[i], gEpsilon);

        result = mat1 / mat2;
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(result[i], mat1[i] / mat2[i], gEpsilon);
    }

    TEST(TestMatrix, Block)
    {
        Matrix4F mat4;
        for (std::size_t i = 0; i < 16; ++i)
            mat4[i] = gUrd(gGen);

        std::size_t startRow = 1, startCol = 1;
        Matrix3F mat3 = mat4.SubMatrix<3, 3>(startRow, startCol);
        for (std::size_t i = 0; i < 3; ++i)
            for (std::size_t j = 0; j < 3; ++j)
                EXPECT_NEAR(mat3(i, j), mat4(startRow + i, startCol + j), gEpsilon);

        Matrix2F mat2;
        for (std::size_t i = 0; i < 4; ++i)
            mat2[i] = gUrd(gGen);
        mat4.SubMatrix<2, 2>(0, 0) = mat2;
        for (std::size_t i = 0; i < 2; ++i)
            for (std::size_t j = 0; j < 2; ++j)
                EXPECT_NEAR(mat4(i, j), mat2(i, j), gEpsilon);
    }

    TEST(TestMatrix, Transpose)
    {
        Matrix4F mat4;
        for (std::size_t i = 0; i < 16; ++i)
            mat4[i] = gUrd(gGen);

        Matrix4F transpose = mat4.Transposed();
        for (std::size_t i = 0; i < 4; ++i)
            for (std::size_t j = 0; j < 4; ++j)
                EXPECT_NEAR(transpose(i, j), mat4(j, i), gEpsilon);

        Matrix4F transpose1 = mat4.Transposed().Transposed();
        for (std::size_t i = 0; i < 4; ++i)
            for (std::size_t j = 0; j < 4; ++j)
                EXPECT_NEAR(transpose1(i, j), mat4(i, j), gEpsilon);
    }

    TEST(TestMatrix, Reduce)
    {
        Matrix4F mat4;
        for (std::size_t i = 0; i < 16; ++i)
            mat4[i] = gUrd(gGen);

        float temp = mat4.Sum();
        float sum = 0.0f;
        for (std::size_t i = 0; i < 16; ++i)
            sum += mat4[i];
        EXPECT_NEAR(sum, temp, gEpsilon);
    }

    TEST(TestMatrix, Dot)
    {
        Vector4F vec0, vec1;
        for (std::size_t i = 0; i < 4; ++i)
        {
            vec0[i] = gUrd(gGen);
            vec1[i] = gUrd(gGen);
        }

        float temp = vec0.Dot(vec1);
        float dot = 0;
        for (std::size_t i = 0; i < 4; ++i)
            dot += vec0[i] * vec1[i];
        EXPECT_NEAR(dot, temp, gEpsilon);
    }

    TEST(TestMatrix, MatrixMultiplication)
    {
        Matrix4F mat1, mat2;
        for (std::size_t i = 0; i < 16; ++i)
        {
            mat1[i] = gUrd(gGen);
            mat2[i] = gUrd(gGen);
        }

        Matrix4F mat3 = mat1 * mat2;
        Matrix4F mat4;
        for (std::size_t i = 0; i < 4; ++i)
        {
            for (std::size_t j = 0; j < 4; ++j)
            {
                mat4(i, j) = 0;
                for (std::size_t k = 0; k < 4; ++k)
                    mat4(i, j) += mat1(i, k) * mat2(k, j);
            }
        }
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(mat3[i], mat4[i], gEpsilon);

        Matrix4F mat5 = mat1.LazyProduct(mat2);
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(mat5[i], mat4[i], gEpsilon);
    }

    TEST(TestMatrix, NullaryOp)
    {
        Matrix4F mat1 = Matrix4F::Identity();
        for (std::size_t i = 0; i < 4; ++i)
        {
            for (std::size_t j = 0; j < 4; ++j)
            {
                if (i == j)
                    EXPECT_NEAR(mat1(i, j), 1.0f, gEpsilon);
                else
                    EXPECT_NEAR(mat1(i, j), 0.0f, gEpsilon);
            }
        }

        mat1 = Matrix4F::Zero();
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(mat1[i], 0.0f, gEpsilon);

        float constant = 2.0f;
        mat1 = Matrix4F::Constant(constant);
        for (std::size_t i = 0; i < 16; ++i)
            EXPECT_NEAR(mat1[i], constant, gEpsilon);

        constant = 3.0f;
        mat1 = constant * Matrix4F::Identity();
        for (std::size_t i = 0; i < 4; ++i)
        {
            for (std::size_t j = 0; j < 4; ++j)
            {
                if (i == j)
                    EXPECT_NEAR(mat1(i, j), constant, gEpsilon);
                else
                    EXPECT_NEAR(mat1(i, j), 0, gEpsilon);
            }
        }

        constant = 4.0f;
        mat1 = Matrix4F::Identity() * constant;
        for (std::size_t i = 0; i < 4; ++i)
        {
            for (std::size_t j = 0; j < 4; ++j)
            {
                if (i == j)
                    EXPECT_NEAR(mat1(i, j), constant, gEpsilon);
                else
                    EXPECT_NEAR(mat1(i, j), 0, gEpsilon);
            }
        }
    }

    TEST(TestMatrix, Determinant)
    {
        Matrix2F mat2a, mat2b;
        for (std::size_t i = 0; i < 4; ++i)
        {
            mat2a[i] = gUrd(gGen);
            mat2b[i] = gUrd(gGen);
        }
        const float result = (mat2a(0, 0) + mat2b(0, 0)) * (mat2a(1, 1) + mat2b(1, 1))
                             - (mat2a(0, 1) + mat2b(0, 1)) * (mat2a(1, 0) + mat2b(1, 0));
        EXPECT_NEAR((mat2a + mat2b).Determinant(), result, gEpsilon);

        Matrix3F mat3;
        for (std::size_t i = 0; i < 9; ++i)
            mat3[i] = 3.0f;
        EXPECT_NEAR(mat3.Determinant(), 0, gEpsilon);
        mat3 = Matrix3F::Identity();
        EXPECT_NEAR(mat3.Determinant(), 1.0f, gEpsilon);

        Matrix4F mat4;
        for (std::size_t i = 0; i < 16; ++i)
            mat4[i] = 4.0f;
        EXPECT_NEAR(mat4.Determinant(), 0, gEpsilon);
        mat4 = Matrix4F::Identity();
        EXPECT_NEAR(mat4.Determinant(), 1.0f, gEpsilon);

        Matrix<float, 5, 5> mat5;
        for (std::size_t i = 0; i < 25; ++i)
            mat5[i] = 5.0f;
        EXPECT_NEAR(mat5.Determinant(), 0, gEpsilon);
        mat5 = Matrix<float, 5, 5>::Identity();
        EXPECT_NEAR(mat5.Determinant(), 1.0f, gEpsilon);
    }

    TEST(TestMatrix, Inverse)
    {
        Matrix2F mat2{
            0, 1,
            2, 3
        };
        Matrix2F invMat2 = mat2.Inversed();
        EXPECT_NEAR(invMat2[0], -1.5f, gEpsilon);
        EXPECT_NEAR(invMat2[1],  0.5f, gEpsilon);
        EXPECT_NEAR(invMat2[2],  1.0f, gEpsilon);
        EXPECT_NEAR(invMat2[3],  0.0f, gEpsilon);

        Matrix3F mat3{
            1.0f, 2.0f, 4.0f,
            1.0f, 3.0f, 5.0f,
            2.0f, 6.0f, 8.0f,
        };
        Matrix3F invMat3 = mat3.Inversed();
        EXPECT_NEAR(invMat3[0],  3.0f, gEpsilon);
        EXPECT_NEAR(invMat3[1], -4.0f, gEpsilon);
        EXPECT_NEAR(invMat3[2],  1.0f, gEpsilon);
        EXPECT_NEAR(invMat3[3], -1.0f, gEpsilon);
        EXPECT_NEAR(invMat3[4],  0.0f, gEpsilon);
        EXPECT_NEAR(invMat3[5],  0.5f, gEpsilon);
        EXPECT_NEAR(invMat3[6],  0.0f, gEpsilon);
        EXPECT_NEAR(invMat3[7],  1.0f, gEpsilon);
        EXPECT_NEAR(invMat3[8], -0.5f, gEpsilon);

        Matrix4F mat4{
            1.0f, 2.0f, 4.0f, 6.0f,
            1.0f, 3.0f, 5.0f, 7.0f,
            2.0f, 6.0f, 8.0f, 10.0f,
            3.0f, 5.0f, 6.0f, 6.0f,
        };
        Matrix4F invMat4 = mat4.Inversed();
        EXPECT_NEAR(invMat4[0],  3.0f, gEpsilon);
        EXPECT_NEAR(invMat4[1], -4.0f, gEpsilon);
        EXPECT_NEAR(invMat4[2],  1.0f, gEpsilon);
        EXPECT_NEAR(invMat4[3],  0.0f, gEpsilon);
        EXPECT_NEAR(invMat4[4],  3.0f, gEpsilon);
        EXPECT_NEAR(invMat4[5], -6.0f, gEpsilon);
        EXPECT_NEAR(invMat4[6],  3.0f, gEpsilon);
        EXPECT_NEAR(invMat4[7], -1.0f, gEpsilon);
        EXPECT_NEAR(invMat4[8], -8.0f, gEpsilon);
        EXPECT_NEAR(invMat4[9],  13.0f, gEpsilon);
        EXPECT_NEAR(invMat4[10], -5.5f, gEpsilon);
        EXPECT_NEAR(invMat4[11],  2.0f, gEpsilon);
        EXPECT_NEAR(invMat4[12],  4.0f, gEpsilon);
        EXPECT_NEAR(invMat4[13], -6.0f, gEpsilon);
        EXPECT_NEAR(invMat4[14],  2.5f, gEpsilon);
        EXPECT_NEAR(invMat4[15], -1.0f, gEpsilon);
    }

    TEST(TestMatrix, Cross)
    {
        Vector2F vec2_0{1, 2}, vec2_1{3, 4};
        EXPECT_NEAR(vec2_0.Cross(vec2_1), -2.0f, gEpsilon);

        Vector3F vec3_0{1, 2, 3}, vec3_1{4, 5, 6};
        Vector3F vec3c = vec3_0.Cross(vec3_1);
        EXPECT_NEAR(vec3c[0], -3.0f, gEpsilon);
        EXPECT_NEAR(vec3c[1],  6.0f, gEpsilon);
        EXPECT_NEAR(vec3c[2], -3.0f, gEpsilon);
    }

    TEST(TestMatrix, Normalize)
    {
        Vector4F vec4{1, 2, 3, 4};
        vec4.Normalize();
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
