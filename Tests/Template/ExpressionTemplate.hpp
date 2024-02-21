// a naive solution( bad! )
// operator overloading
#include <iostream>
#include <algorithm>
#include <cassert>

class MyType 
{
public:
    explicit MyType(std::size_t size, double val = 0.0) : m_size(size)
    {
        m_features = new double [m_size];
        for (std::size_t i = 0; i < m_size; i++) m_features[i] = val;
        std::cout << "\tMyType constructor Size = " << m_size << "\n";
    }

    // construct vector using initializer list 
    MyType(std::initializer_list<double> init) 
    {
        m_size = init.size();
        m_features = new double[m_size];
        for (std::size_t i = 0; i < m_size; i++) m_features[i] = *(init.begin() + i);
        std::cout << "\tMyType constructor Size = " << m_size << "\n";
    }
    // in c++11 move constructor can be used here 
    MyType(const MyType& rhs): m_size(rhs.Size())
    {
        m_features = new double[m_size];
        for (std::size_t i = 0; i < m_size; i++) m_features[i] = rhs[i];
        std::cout << "\tMyType copy constructor Size = " << m_size << "\n";
    }

    MyType& operator=(const MyType& rhs)
    {
        if (this != &rhs) 
        {
            delete[]m_features;
            m_size = rhs.Size();
            m_features = new double[m_size];
            for (std::size_t i = 0; i < m_size; i++) m_features[i] = rhs[i];
            std::cout << "\tMyType assignment constructor Size = " << m_size << "\n";
        }
        return *this;
    }

    ~MyType() 
    {
        if (nullptr != m_features)
        {
            delete [] m_features;
            m_size = 0;
            m_features = nullptr;
        }
    }

    double &operator[](std::size_t i) { return m_features[i]; }
    double operator[](std::size_t i) const { return m_features[i]; }
    [[nodiscard]] std::size_t Size() const { return m_size; }
private:
    std::size_t m_size;
    double* m_features;
};

//This kind of approach is inefficient, because temporal memory is allocated and de-allocated during each operation
MyType operator+(MyType const &u, MyType const &v)
{
    MyType sum(std::max(u.Size(), v.Size()));
    for (std::size_t i = 0; i < u.Size(); i++)
        sum[i] = u[i] + v[i];
    std::cout << "\t in MyType +\n";
    return sum;
}
// operator- balabala

template<typename A>
struct Expression
{
    const A& Cast() const { return static_cast<const A&>(*this); }
    [[nodiscard]] int Size() const { return Cast().Size(); }
private:
    Expression& operator=(const Expression&) { return *this; }
    Expression() = default;
    friend A;
};

template<typename Func, typename TLhs, typename TRhs>
struct BinaryOp : public Expression<BinaryOp<Func, TLhs, TRhs>>
{
    BinaryOp(const TLhs& lhs, const TRhs& rhs) : m_lhs(lhs.Cast()), m_rhs(rhs.Cast())
    {
        std::cout << "BinaryOp constructor" << std::endl;
    }

    [[nodiscard]] double Value(int i) const
    {
        return Func::Op(m_lhs.Value(i), m_rhs.Value(i));
    }

    [[nodiscard]] int Size() const { return std::max(m_lhs.Size(), m_rhs.Size()); }

private:
    const TLhs& m_lhs;
    const TRhs& m_rhs;
};

template<typename Func, typename TLhs, typename TRhs>
inline BinaryOp<Func, TLhs, TRhs>
ExpToBinaryOp(const Expression<TLhs>& lhs, const Expression<TRhs>& rhs)
{
    return BinaryOp<Func, TLhs, TRhs>(lhs.Cast(), rhs.Cast());
}

struct Add
{
    static double Op(double a, double b) { return a + b; }
};

struct Minimum
{
    static double Op(double a, double b) { return a < b ? a : b; }
};

template<typename TLhs, typename TRhs>
inline BinaryOp<Add, TLhs, TRhs>
operator+(const Expression<TLhs>& lhs, const Expression<TRhs>& rhs)
{
    return ExpToBinaryOp<Add>(lhs, rhs);
}

template<typename TLhs, typename TRhs>
inline BinaryOp<Minimum, TLhs, TRhs>
Min(const Expression<TLhs>& lhs, const Expression<TRhs>& rhs)
{
    return ExpToBinaryOp<Minimum>(lhs, rhs);
}

// allocation just by user
// no constructor and destructor to allocate and de-allocate memory
class MyExpType : public Expression<MyExpType>
        {
public:
    MyExpType(): m_size(0), m_features(nullptr) {}
    MyExpType(double *features, int size)
            : m_size(size), m_features(features) {
        printf("MyExpType constructor size = %d. No memory allocate.\n", m_size);
    }

    // delete copy constructor,
    MyExpType(const MyExpType& src) = delete;
    template<typename ExpType>
    MyExpType(const Expression<ExpType>& src) = delete;

    // here is where computing happens,lazy support
    template<typename ExpType>
    MyExpType& operator=(const Expression<ExpType>& src) {
        const ExpType &srcReal = src.Cast();
        assert(m_size >= srcReal.Size()); //
        for (int i = 0; i < srcReal.Size(); ++i)
            m_features[i] = srcReal.Value(i); // binary expression value work function

        printf("MyExpType assignment constructor size = %d\n", m_size);
        return *this;
    }
    // computing function
    [[nodiscard]] double Value(int i) const { return m_features[i]; }
    int Size() const { return m_size; }

private:
    int m_size;
    double* m_features;
};

void Print(const MyExpType& m) {
    printf("( ");
    for (int i = 0; i < m.Size() - 1; ++i) {
        printf("%g, ", m.Value(i));
    }
    if (m.Size())
        printf("%g )\n", m.Value(m.Size()-1));
    else
        printf(" )\n");
}