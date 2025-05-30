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

    std::function<char(int, char)> l = [](int x, char y)
    { return 'a'; };

    using t = return_type_t<decltype(f)>;

    constexpr auto lam1 = [](int x, int y) -> int
    {
        return 3;
    };

    constexpr auto lam2 = [](int a, int b) -> int
    {
        return 4;
    };

    static_assert(std::is_same_v<t, int>);
    static_assert(!is_same_return_type_v<decltype(test_f), decltype(f)>);
    static_assert(is_same_return_type_callable_v<decltype(lam1), decltype(lam2), int, int>);
    static_assert(is_same_signature<decltype(f), decltype(g), int, int>::value);
    static_assert(!is_same_signature<decltype(f), decltype(l), int, int>::value);

    return 0;
}