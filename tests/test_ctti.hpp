#pragma once
#include "../src/templa.hpp"

namespace templa::test
{
    static_assert(type_info::ctti::hash_t<int>::id != type_info::ctti::hash_t<char>::id);

    // autological
    static_assert(type_info::ctti::hash_t<int>::id == type_info::ctti::hash_t<int>::id);
};
