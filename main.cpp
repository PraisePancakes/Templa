#include <iostream>
#include "src/templa.hpp"
#include "tests/test_algorithms.hpp"
#include "tests/test_type_list.hpp"
#include "tests/test_ctti.hpp"
#include <vector>

int main(int argc, char **argv)
{
    using namespace templa;
    int x = 3;
    int y = 3;
    std::string z = "z";

    try_call([](int a, int b)
             { std::cout << a + b; }, x, y);

    return 0;
}