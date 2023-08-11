#include "mustache/mustache.hpp"
#include <iostream>

using namespace kainjow::mustache;

int main()
{
    mustache tmpl{"Hello {{what}}!"};
    std::cout << tmpl.render({"what", "World"}) << std::endl;
    return 0;
}