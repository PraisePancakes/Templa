#pragma once
#include "../src/templa.hpp"

namespace templa::test
{
    static_assert(ctti::hash_t<int>::id != ctti::hash_t<char>::id);

    // autological
    static_assert(ctti::hash_t<int>::id == ctti::hash_t<int>::id);
};
