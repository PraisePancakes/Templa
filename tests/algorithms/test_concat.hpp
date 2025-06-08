#pragma once
#include "../../src/templa.hpp"

namespace templa::test
{
    void test_concat()
    {
        constexpr std::array<int, 2>
            lhs{1, 2};
        constexpr std::array<int, 3> rhs{1, 2, 3};

        constexpr auto narr = algorithms::concat(lhs, rhs);

        static_assert(narr[0] == 1);
        static_assert(narr[1] == 2);
        static_assert(narr[2] == 1);
        static_assert(narr[3] == 2);
        static_assert(narr[4] == 3);
    };

};
