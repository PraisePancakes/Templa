#include <iostream>
#include "src/templa.hpp"
#include "tests/test_algorithms.hpp"
#include "tests/test_type_list.hpp"
#include "tests/test_ctti.hpp"

int main(int argc, char **argv)
{
    using namespace templa;
    std::cout << utils::type_descriptor<const int **>::stringify();
    return 0;
}