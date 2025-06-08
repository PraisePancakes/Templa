#pragma once
#include "../../src/templa.hpp"

namespace templa::test
{
    struct A
    {
        int val;
        constexpr A() : val{} {};
        constexpr A(int v) : val{v} {};
        constexpr bool operator==(const A &o) const { return this->val == o.val; };
        constexpr bool operator!=(const A &o) const { return !(this->val == o.val); };
        constexpr bool operator<(const A &o) const { return this->val < o.val; };
        constexpr bool operator>(const A &o) const { return this->val > o.val; };
        constexpr bool operator<=(const A &o) const { return this->val <= o.val; };
        constexpr bool operator>=(const A &o) const { return this->val >= o.val; };
        constexpr int operator*() const { return val; };
        ~A() = default;
    };

    void test_max()
    {
        constexpr A a{1};
        constexpr A b{2};
        constexpr A c{3};

        constexpr std::array<A, 3> arr{a, b, c};

        static_assert(templa::algorithms::max_from<arr>::value == c);
    };
};
