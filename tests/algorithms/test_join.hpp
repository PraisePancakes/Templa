#pragma once
#include "../../src/templa.hpp"

namespace templa::test
{
    void test_join()
    {

        constexpr static std::string_view str1 = "hi";
        constexpr static std::string_view str2 = "bob";

        constexpr static std::string_view s = algorithms::join_v<str1, str2>;
        static_assert(s == "hibob");
    };

};
