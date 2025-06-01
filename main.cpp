#include <iostream>
#include "src/templa.hpp"
#include "tests/test_algorithms.hpp"
#include "tests/test_type_list.hpp"
#include "tests/test_ctti.hpp"
#include <vector>
#include <functional>

class A
{

public:
    int x, y;
    constexpr A(int x, int y) : x(x), y(y) {};
};

int main(int argc, char **argv)
{
    using namespace templa;
    templa::test::TEST();

    constexpr std::tuple<int, char, double> t1 = {1, 'a', 3.14};
    constexpr std::tuple<float, bool, const char *> t2 = {2.5f, true, "hello"};

    constexpr auto zipped = templa::algorithms::zip(t1, t2);

    static_assert(std::get<0>(zipped).first == 1);

    return 0;
}