#include <iostream>
#include "src/templa.hpp"
#include "tests/test_algorithms.hpp"
#include "tests/test_type_list.hpp"
#include "tests/test_ctti.hpp"
#include <vector>
#include <functional>

int test_f(int x, int y) { return 4; };

int main(int argc, char **argv)
{
    using namespace templa;

    std::function<int(int, int)> f = [](int x, int y)
    { return x + y; };

    using t = return_type_t<decltype(test_f)>;
    static_assert(std::is_same_v<t, int>);
    return 0;
}