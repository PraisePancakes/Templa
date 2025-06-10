#pragma once
#include "../../src/templa.hpp"

namespace templa::test
{

    void test_unique()
    {
        constexpr auto unique_list = algorithms::unique<1, 1, 2, 1, 3, 3>::unique_sequence;
        static_assert(unique_list[0] == 1);
        static_assert(unique_list[1] == 2);
        static_assert(unique_list[2] == 3);
        static_assert(unique_list.size() == 3);

        constexpr std::array<int, 5> array{1, 2, 2, 1, 3};

        constexpr auto unique_list2 = algorithms::unique_from<array>::unique_sequence;
        static_assert(unique_list2[0] == 1);
        static_assert(unique_list2[1] == 2);
        static_assert(unique_list2[2] == 3);
        static_assert(unique_list2.size() == 3);
    };

};
