int test()
{
    return 0;
}

#define REFLECT clang::annotate("reflect")

class [[REFLECT]] Base
{
public:
    Base();
    virtual ~Base() = default;

    virtual void Function();

protected:
    void Function2();

private:
    void Function3();
};

class Derived : public Base
{

};

namespace TG
{
    struct Base
    {
        void Function();
    };
}
