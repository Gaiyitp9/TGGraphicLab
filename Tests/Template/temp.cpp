
// #include <iostream>
// #include <type_traits>
// #include <concepts>
//
// template <typename T>
// class Wrapper;
// // {
// // public:
// //     // 构造函数
// //     // template <typename U>
// //     // Wrapper(U&& value) : value_(std::forward<U>(value)) {
// //     //     std::cout << __FUNCSIG__ << std::endl;
// //     //     // 根据 T 是否是 const 来选择不同的行为
// //     //     if constexpr (std::is_const_v<std::remove_reference_t<U>>) {
// //     //         std::cout << "Constructed with const reference" << std::endl;
// //     //     } else {
// //     //         std::cout << "Constructed with non-const reference" << std::endl;
// //     //     }
// //     // }
// //     Wrapper(T value) : value_(value)
// //     {
// //         std::cout << __FUNCSIG__ << std::endl;
// //     }
// //
// //     T getValue() const { return value_; }
// //
// // private:
// //     T value_;
// // };
//
// template<typename U> Wrapper(U&) -> Wrapper<U>;
// // template<typename U> Wrapper(const U&) -> Wrapper<const U&>;
//
// template <>
// class Wrapper<const int>
// {
// public:
//     Wrapper(const int& value) : value_(value)
//     {
//         std::cout << __FUNCSIG__ << std::endl;
//     }
//
//     const int& getValue() const { return value_; }
//
// private:
//     const int& value_;
// };
//
// template <>
// class Wrapper<int>
// {
// public:
//     Wrapper(int& value) : value_(value)
//     {
//         std::cout << __FUNCSIG__ << std::endl;
//     }
//
//     const int& getValue() const { return value_; }
//
// private:
//     int& value_;
// };
//
//
// int main()
// {
//     int x0 = 10;
//     int& x = x0;
//     const int& y = 20;
//
//     Wrapper wrap1(x);    // 调用非 const 引用构造函数
//     Wrapper wrap2(y);    // 调用 const 引用构造函数
//
//     std::cout << wrap1.getValue() << std::endl; // 输出: 10
//     std::cout << wrap2.getValue() << std::endl; // 输出: 20
//
//     return 0;
// }

#include <iostream>

// 模板类
// template<int N>
// class Deep {
// public:
//     // 模板成员函数
//     template<typename T>
//     void f();
// };
//
// // 模板成员函数的定义
// template<int N>
// template<typename T>
// void Deep<N>::f() {
//     std::cout << "Template member function f() called with T" << std::endl;
// }
//
// int main() {
//     // 实例化模板类
//     Deep<10> d;
//
//     // 调用模板成员函数
//     d.f<int>();
//
//     return 0;
// }

#include <iostream>

// consteval int square(int x) {
//     return x * x;
// }
//
// template<typename T>
// class TestConst
// {
// public:
//     explicit TestConst(int x) : m_x(x) {}
//
//     const int& Num() const
//     {
//         T const* t = static_cast<T const*>(this);
//         std::cout << __FUNCSIG__ << std::endl;
//         return m_x;
//     }
//     int Num()
//     {
//         std::cout << __FUNCSIG__ << std::endl;
//         return m_x;
//     }
//
// private:
//     int& m_x;
// };
//
// class TestDerived : public TestConst<TestDerived>
// {
// public:
//     explicit TestDerived(int x) : TestConst(x) {}
// };
//
// template<typename T>
// class ReferenceTest
// {
// public:
//     explicit ReferenceTest(T& xpr) : m_xpr(xpr) {}
//
//     const T& Expression() const
//     {
//         std::cout << __FUNCSIG__ << std::endl;
//         return m_xpr;
//     }
//
// private:
//     T& m_xpr;
// };
//
// int main()
// {
//     constexpr int result = square(5); // 编译时求值
//     std::cout << "Square of 5 is: " << result << std::endl;
//
//     // 下面这行是错误的，因为consteval函数不能在运行时调用
//     int runtime_result = square(7); // 这会导致编译错误
//     std::cout << "Square of 7 is: " << runtime_result << std::endl;
//
//     int x = 5;
//     const TestDerived constant(x);
//     int num = constant.Num();
//     std::cout << num << std::endl;
//
//     const int& x2 = 4;
//     ReferenceTest ref(x2);
//     std::cout << ref.Expression() << std::endl;
//
//     return 0;
// }

#include <concepts>

template <typename T> requires std::integral<T> && requires { typename T::value_type; }
struct Base {
    void baseMethod() {}
};

template <typename T> requires std::integral<T>
struct Derived : Base<T> {
    void derivedMethod() {}
};

int main()
{
    Derived<int> i;
}