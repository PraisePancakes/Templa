#pragma once
#include "../../src/templa.hpp"

namespace templa::test
{
    void test_reverse()
    {
        constexpr auto reverse_list = algorithms::reverse<1, 1, 2, 1, 3, 3>::reverse_sequence;
        static_assert(reverse_list[0] == 3);
        static_assert(reverse_list[1] == 3);
        static_assert(reverse_list[2] == 1);
        static_assert(reverse_list[3] == 2);
        static_assert(reverse_list[4] == 1);
        static_assert(reverse_list[5] == 1);
        static_assert(reverse_list.size() == 6);

        constexpr std::array<int, 5> arr{0, 1, 2, 3, 4};
        constexpr auto reverse_from_list = algorithms::reverse_from<arr>::reverse_sequence;

        static_assert(reverse_from_list[0] == 4);
        static_assert(reverse_from_list[1] == 3);
        static_assert(reverse_from_list[2] == 2);
        static_assert(reverse_from_list[3] == 1);
        static_assert(reverse_from_list[4] == 0);

        static_assert(reverse_from_list.size() == 5);
    };

};
