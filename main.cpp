#include <iostream>
#include "src/templa.hpp"
#include "tests/algorithms/test_concat.hpp"
#include "tests/test_type_list.hpp"
#include "tests/test_ctti.hpp"

int main(int argc, char **argv)
{
  using namespace templa;

  (void)algorithms::unique<1, 1, 3>::unique_array;

  return 0;
}