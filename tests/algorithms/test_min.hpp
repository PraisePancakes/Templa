#pragma once
#include "../../src/templa.hpp"

namespace templa::test
{

    void test_min()
    {
        constexpr A a{1};
        constexpr A b{2};
        constexpr A c{3};

        constexpr std::array<A, 3> arr{a, b, c};

        static_assert(templa::algorithms::min<1, 2, 3>::value == 1);
        static_assert(templa::algorithms::min<a, b, c>::value == a);
        static_assert(templa::algorithms::min_from<arr>::value == a);
    };
};
