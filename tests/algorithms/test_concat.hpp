#pragma once
#include "../../src/templa.hpp"

namespace templa::test
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

    constexpr static std::string_view str1 = "hi";
    constexpr static std::string_view str2 = "bob";

    constexpr static std::string_view s = algorithms::join_v<str1, str2>;
    static_assert(s == "hibob");
};
