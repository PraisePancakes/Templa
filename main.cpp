#include <iostream>
#include "src/templa.hpp"
#include "tests/test_algorithms.hpp"
#include "tests/test_type_list.hpp"
#include "tests/test_ctti.hpp"
#include <vector>

int main(int argc, char **argv)
{
    using namespace templa;
    std::vector<int> arr{0, 1, 2, 3};

    std::vector<std::vector<std::vector<int>>> v{{{0, 1, 2}, {1, 2}}};
    std::vector<int> v2;
    algorithms::flatten(v, v2);

    for (const auto &e : v2)
    {
        std::cout << e;
    }

    return 0;
}