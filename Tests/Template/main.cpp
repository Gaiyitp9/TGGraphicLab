#include <type_traits>
#include <iostream>
#include <tuple>
#include "Base/ConstString.hpp"
#include "ExpressionTemplate.hpp"

//-----------------------------------------------------------------
// 测试模板参数推导
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
//-----------------------------------------------------------------

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

template<char c>
struct TypeContainer
{

};

template<int N>
constexpr int Sum()
{
    int sum = 0;
    for (int i = 0; i < N; ++i)
        sum += i;
    return sum;
}

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

template<std::size_t N0, std::size_t... Ns>
void IntList3(std::index_sequence<N0, Ns...>)
{
    std::cout << N0 << ' ';
    if constexpr (sizeof...(Ns) > 0)
        IntList3(std::index_sequence<Ns...>());
    std::cout << std::endl;
}

//-----------------------------------------------------------------
// template two-phase lookup
template<typename Derived>
class Base
{
public:
    using Type = int;

protected:
    int num = 2;
};

template<typename T>
class Derived1 : public Base<Derived1<T>>
{
    using Base = Base<Derived1<T>>;
public:
    // 必须添加Base::或者this->使得num变成qualified dependent name，lookup会推迟到第二阶段，即实例化阶段
    Derived1() { std::cout << Base::num << std::endl; }

    Base::Type i = 0;
};
//-----------------------------------------------------------------

class MyBase
{
public:
    // template<typename T>
    MyBase& operator=(const MyBase&)
    {
        std::cout << "MyBase operator=" << std::endl;
        return *this;
    }
};

class MyDerived : public MyBase
{
public:
    MyDerived() = default;
    MyDerived(const MyDerived&)
    {
        std::cout << "MyDerived copy constructor" << std::endl;
    }
    // template<typename T>
    // MyDerived& operator=(const T&)
    // {
    //     std::cout << "MyDerived operator=" << std::endl;
    //     return *this;
    // }
    // using MyBase::operator=;
    // MyDerived(MyDerived&&) = delete;
    // MyDerived& operator=(const MyDerived&)
    // {
    //     std::cout << "MyDerived operator=" << std::endl;
    //     return *this;
    // }
};

class MiniMatrix
{
public:
    MiniMatrix() {}

    int operator[](int index) const { return m_data[index]; }
    int& operator[](int index) { return m_data[index]; }

private:
    int m_data[4] { 0, 1, 2, 3 };
};

class TestConstFunction
{
public:
    explicit TestConstFunction(int& i, MiniMatrix& matrix) : m_i(i), m_matrix(matrix) {}

    int& GetNum() { return m_i; }
    [[nodiscard]] int GetNum() const { return m_i; }
    int& GetEntry(int index) { return m_matrix[index]; }
    [[nodiscard]] int GetEntry(int index) const { return m_matrix[index]; }

private:
    int& m_i;
    MiniMatrix& m_matrix;
};

template <typename T>
constexpr auto addLambda = [](T a, T b) -> T {
    return a + b;
};

int main()
{
    MiniMatrix matrix;
    int myii = 2;
    const TestConstFunction fffff(myii, matrix);
    int num = fffff.GetNum();
    int entry = fffff.GetEntry(2);

    int const* ci = new int[3] { 1, 2, 3 };
    int* nonci = const_cast<int*>(ci);
    int* ai = new int[3] { 1, 2, 3 };
    int& rc = ai[0];
    nonci[0] = 4;
    rc = 2;

    MyDerived dd;
    MyDerived dd1;
    dd1 = dd;
    // [const] [volatile] <type> [*|&] [[N]]碰到T，会退化(decay)为 <type> [*](当类型为指针或数组时)
    // [const] [volatile] <type> [*|&] [[N]]碰到T&，不变
    // 注：[]内表示可能出现的词，<>内表示一定要出现的词
    // 下面是几个简单的示例
    const int inum = 2;
    char const* str = "what";
    char const str2[5] = "what";
    const char& ch = 's';
    std::string str3 = "what?";
    // T&
    Test(inum);     // const int -> const int
    Test(str);      // char const* -> char const*
    Test(str2);     // char const[N] -> char const[N]
    Test(ch);       // const char& -> const char
    // const T&
    Test1(inum);    // const int -> int
    Test1(str);     // char const* -> char const*
    Test1(str2);    // char const[N] -> char[N]
    Test1(ch);      // const char& -> char
    Test1(str3);      // const char& -> char
    // T
    Test2(inum);    // const int -> int
    Test2(str);     // char const* -> char const*
    Test2(str2);    // char const[N] -> char const*
    Test2(ch);      // const char& -> char
    // const T
    Test3(inum);    // const int -> int
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
    constexpr TG::ConstString cstr("what");

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

    //-----------------------------------------------------------------------------------------------
    // 测试decltype
    std::tuple<int, float, TypeContainer<'a'>> types;
    std::cout << typeid(decltype(std::get<2>(types))).name() << std::endl;

    //-----------------------------------------------------------------------------------------------
    // 编译期for测试
    // for的i不能用在模板参数上
    static_assert(Sum<5>() == 10);

    //-----------------------------------------------------------------------------------------------
    IntList(std::make_index_sequence<1>());
    IntList2(std::make_index_sequence<0>());
    IntList3(std::make_index_sequence<3>());

    //-----------------------------------------------------------------------------------------------

    MyType a(3, 1.1);
    MyType b(3, 2.01);
    MyType c = {3.01, 3.01, 3.01};

    std::cout << "\t----computing-----\n";
    MyType d = a + b + c;
    for (size_t i = 0; i < d.Size(); i++)
    {
        std::cout << "\t" << d[i] << "\n";
    }

    const int N = 3;
    double sa[N] = { 1.1,1.1, 1.1 };
    double sb[N] = { 2.01, 2.01, 2.01 };
    double sc[N] = { 3.01, 3.01, 3.01 };
    double sd[N] = { 0 };
    MyExpType A(sa, N), B(sb, N), C(sc, N), D(sd, N);
    printf("\n");
    printf(" A = ");
    Print(A);
    printf(" B = ");
    Print(B);
    printf(" C = ");
    Print(C);
    printf("\n\tD = A + B + C\n");
    D = A + B + C;
    for (int i = 0; i < A.Size(); ++i) {
        printf("%d:\t%g + %g + %g = %g\n",
               i, B.Value(i), C.Value(i), B.Value(i), D.Value(i));
    }
    printf("\n\tD = A + min(B, C)\n");
    // D = A + expToBinaryOp<Minimum>(B, C);
    D = A + Min(B, C);
    for (int i = 0; i < A.Size(); ++i) {
        printf("%d:\t%g + min(%g, %g) = %g\n",
               i, A.Value(i), B.Value(i), C.Value(i), D.Value(i));
    }


    Derived1<float> ddd;
    Derived1<float>::Type i;

    return 0;
}
