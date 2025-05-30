#include <iostream>
#include "src/templa.hpp"
#include "tests/test_algorithms.hpp"
#include "tests/test_type_list.hpp"
#include "tests/test_ctti.hpp"
#include <vector>

int main(int argc, char **argv)
{
    using namespace templa;

    static_assert(is_callable<decltype([](int, int) {}), int, int>::value);

    return 0;
}