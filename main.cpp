#include <iostream>
#include "src/templa.hpp"
#include "tests/test_algorithms.hpp"
#include "tests/test_type_list.hpp"
#include "tests/test_ctti.hpp"
#include <vector>
#include <functional>

int main(int argc, char **argv)
{
    using namespace templa;
    templa::test::TEST();

    auto lam = [](auto &&...args)
    {
        ((std::cout << std::forward<decltype(args)>(args) << "\n"), ...);
    };

    auto t1 = std::make_tuple(1, 'a');
    auto t2 = std::make_tuple(2, 'b');

    templa::algorithms::apply_to_tuple_cat(lam, t1, t2);
    return 0;
}