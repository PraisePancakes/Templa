#include <iostream>
#include "src/templa.hpp"
#include "tests/test_algorithms.hpp"
#include "tests/test_type_list.hpp"
#include "tests/test_ctti.hpp"
#include <vector>
#include <functional>

char test_f(int x, int y) { return 4; };

int main(int argc, char **argv)
{
    using namespace templa;

    std::function<int(int, int)> f = [](int x, int y)
    { return x + y; };

    std::function<int(int, int)> g = [](int x, int y)
    { return x + y; };

    using t = return_type_t<decltype(f)>;

    static_assert(std::is_same_v<t, int>);
    static_assert(!is_same_return_type_v<decltype(test_f), decltype(f)>);

    return 0;
}