#include <iostream>
#include "Math/Core.hpp"

int main()
{
    TG::Math::Matrix4f v1, v2, v3;
    v1(0, 0) = v1(1, 1) = v1(2, 2) = v1(3, 3) = 1;
    v2(0, 0) = v2(1, 1) = v2(2, 2) = v2(3, 3) = 3;
    v3(0, 0) = v3(1, 1) = v3(2, 2) = v3(3, 3) = 2;
    TG::Math::Matrix4f a = v1.CWiseProduct(v2);
    TG::Math::Matrix4f b = v1 + v2 - v3;
    b(3, 3) = -1;
    TG::Math::Matrix3f c = b.Block<3, 3>(1, 1);

    std::cout << "sum of a: " << a.Sum() << '\n';
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            std::cout << b(i, j) << ' ';
        }
        std::cout << '\n';
    }
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            std::cout << c(i, j) << ' ';
        }
        std::cout << '\n';
    }
    return 0;
}
