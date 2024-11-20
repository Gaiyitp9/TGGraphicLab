#include <functional>
#include <iostream>
#include <print>

int f(int a) { return -a; }
void g(double) {}
class Test
{
public:
    void f(double) {}
    virtual void g(double) { std::cout << "Test!" << std::endl; }
};

class Derived : public Test
{
public:
    void g(double) override { std::cout << "Derived!" << std::endl; }
    void TestBind(double d0, double d1, double d2) { std::print("d0 = {}, d1 = {}, d2 = {}", d0, d1, d2); }
};

void BindTest(double d0, double d1, double d2)
{
    std::print("d0 = {}, d1 = {}, d2 = {}\n", d0, d1, d2);
}

int main()
{
    std::function<void(double)> delegate = std::bind_back(BindTest, 1.0, 5.0);
    delegate(1.0);
    Derived d0;
    std::function<void(double)> delegate1 = std::bind_back([&d0](double a) {
        std::invoke(&Derived::TestBind, &d0, a, 1.0, 2.0);
    });
    delegate1(1.0);
    // fn1 and fn2 have the same type, but their targets do not
    std::function<int(int)> fn1(f);
    std::function fn2([](int a) {return -a;});
    std::function<void(Test&, double)> fn3(&Test::f);
    std::function<void(Test&, double)> fn4(&Test::f);
    std::function<void(Test&, double)> fn5(&Test::g);
    std::function<void(Test&, double)> fn6(&Test::g);
    std::function fn7([](double a) { std::cout << a << std::endl; });
    std::function fn8(g);
    Derived d;
    std::function<void(double)> fn9 = std::bind(&Test::g, d, std::placeholders::_1);
    fn5(d, 3.0);
    fn6(d, 4.0);
    fn7(2.0);
    fn8(5.0);
    fn9(2);
    std::cout << fn1.target_type().name() << '\n'
              << fn2.target_type().name() << '\n'
              << fn3.target_type().name() << '\n';

    auto p1 = fn1.target<int(*)(int)>();
    auto p3 = fn3.target<void(Test::*)(double)>();
    auto p4 = fn4.target<void(Test::*)(double)>();
    auto p5 = fn5.target<void(Test::*)(double)>();
    auto p6 = fn6.target<void(Test::*)(double)>();
    auto p7 = fn7.target<void(double)>();
    auto p8 = fn8.target<void(*)(double)>();

    std::cout << "fn3 fn4 " << std::boolalpha << (fn3.target_type() == fn4.target_type()) << " "
        << (p3 == p4) << std::endl;
    std::cout << "fn5 fn6 " << std::boolalpha << (fn5.target_type() == fn6.target_type()) << " "
        << (p5 == p6) << std::endl;
    std::cout << "fn7 fn8 " << std::boolalpha << (fn7.target_type() == fn8.target_type());

    // since C++17 deduction guides (CTAD) can avail
    std::cout << std::function{g}.target_type().name() << '\n';
}
