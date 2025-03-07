#include <functional>
#include <print>
#include <utility>
#include <string>
#include <tuple>
#include "Base/Delegate.hpp"

void Print(int& num)
{
    std::println("int& {}", num);
}

void Print(int&& num)
{
    std::println("int&& {}", num);
}

void Print(int&& num, int num1)
{
    std::println("int&& {} int {}", num, num1);
}

void Print(int&& num, const int num1, int num2, int& num3)
{
    std::println("int&& {} const int {} int {} int& {}", num, num1, num2, num3);
}

template <typename... Args>
auto MakeLambda(Args&&... args)
{
    // 使用 forward_as_tuple 保留参数的引用类型
    return [args = std::forward_as_tuple(std::forward<Args>(args)...)]() mutable {
        // 解包时正确传递值类别
        std::apply(
            []<typename... T>(T&&... unpackedArgs) {
                (Print(std::forward<T>(unpackedArgs)), ...);
            },
            std::move(args) // 传递元组的所有权
        );
    };
}

int f(int a)
{
    return -a;
}
void g(double)
{

}
class Test
{
public:
    void f(double) {}
    virtual void g(double) { std::println("Test!"); }
};

class Derived : public Test
{
public:
    void g(double) override { std::println("Derived!"); }
    void TestBind(double d0, double d1, double d2) { std::println("d0 = {}, d1 = {}, d2 = {}", d0, d1, d2); }
};

void BindTest(double d0, double d1, double d2)
{
    std::println("d0 = {}, d1 = {}, d2 = {}", d0, d1, d2);
}

int main()
{
    std::function<void(double)> function0 = std::bind_back(BindTest, 1.0, 5.0);
    function0(1.0);
    Derived d0;
    std::function<void(double)> function1 = std::bind_back([&d0](double a) {
        std::invoke(&Derived::TestBind, &d0, a, 1.0, 2.0);
    });
    function1(1.0);
    // fn1 and fn2 have the same type, but their targets do not
    std::function<int(int)> fn1(f);
    std::function fn2([](int a) {return -a;});
    std::function<void(Test&, double)> fn3(&Test::f);
    std::function<void(Test&, double)> fn4(&Test::f);
    std::function<void(Test&, double)> fn5(&Test::g);
    std::function<void(Test&, double)> fn6(&Test::g);
    std::function fn7([](double a) { std::println("{}", a); });
    std::function fn8(g);
    Derived d;
    std::function<void(double)> fn9 = std::bind(&Test::g, d, std::placeholders::_1);
    fn5(d, 3.0);
    fn6(d, 4.0);
    fn7(2.0);
    fn8(5.0);
    fn9(2);
    std::print("{}\n{}\n{}\n", fn1.target_type().name(),
        fn2.target_type().name(), fn3.target_type().name());

    auto p3 = fn3.target<void(Test::*)(double)>();
    auto p4 = fn4.target<void(Test::*)(double)>();
    auto p5 = fn5.target<void(Test::*)(double)>();
    auto p6 = fn6.target<void(Test::*)(double)>();

    std::println("fn3 fn4 {} {}", fn3.target_type() == fn4.target_type(), p3 == p4);
    std::println("fn5 fn6 {} {}", fn5.target_type() == fn6.target_type(), p5 == p6);
    std::println("fn7 fn8 {}", fn7.target_type() == fn8.target_type());

    // since C++17 deduction guides (CTAD) can avail
    std::println("{}", std::function{g}.target_type().name());

    std::string str = "Hello World!";
    int i = 1;
    int& j = i;
    auto lambda = MakeLambda(j);
    auto lambda1 = MakeLambda(2, i);
    lambda();
    lambda1();

    TG::Delegate<void(int&&)> delegate0;
    delegate0.Bind(Print);
    delegate0.Execute(2);

    TG::Delegate<void(int&&)> delegate1 = [](int&& num) {
        std::println("Hello Delegate! {}", num);
    };
    delegate1.Execute(2);

    Derived d1;
    TG::Delegate<void()> delegate2;
    delegate2.Bind(&Test::g, &d1, 3.0);
    delegate2.Execute();
    TG::Delegate<void(double, double)> delegate3;
    delegate3.Bind(&Derived::TestBind, &d1, 3.0);
    delegate3.Execute(2, 2);

    TG::Delegate<void(double, double)> delegate4(BindTest, 4.0);
    delegate4.Execute(2, 3);

    TG::MulticastDelegate<void()> multicastDelegate;
    multicastDelegate.Add(delegate2);
    multicastDelegate.Add(BindTest, 7.0, 8.0, 9.0);
    multicastDelegate.Broadcast();
    multicastDelegate.Remove(delegate2);
    multicastDelegate.Broadcast();

    return 0;
}
