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

    constexpr std::array<A, 2> arr1{A(1, 2), A(2, 3)};
    constexpr std::array<A, 1> arr2{A(3, 4)};

    constexpr auto c1 = templa::algorithms::concat(arr1, arr2);

    static_assert(c1[0].x == 1);
    static_assert(c1[1].x == 2);

    constexpr std::array<int, 2> arr3{1, 2};
    constexpr std::array<int, 1> arr4{3};

    constexpr auto c2 = templa::algorithms::concat(arr3, arr4);

    static_assert(c2[0] == 1);
    static_assert(c2[1] == 2);

    return 0;
}