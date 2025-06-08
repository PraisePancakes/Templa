#include <iostream>
#include "src/templa.hpp"
#include "tests/algorithms/test_concat.hpp"
#include "tests/algorithms/test_max.hpp"
#include "tests/test_type_list.hpp"
#include "tests/test_ctti.hpp"

int main(int argc, char **argv)
{
  templa::test::test_max();

  return 0;
}