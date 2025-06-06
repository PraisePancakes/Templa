#include <iostream>
#include "src/templa.hpp"
#include "tests/test_algorithms.hpp"
#include "tests/test_type_list.hpp"
#include "tests/test_ctti.hpp"

int main(int argc, char **argv)
{
  using namespace templa;

  constexpr std::array<int, 3> arr{0, 1, 1};

  (void)templa::algorithms::forward_elements_from<arr>::value;

  (void)templa::algorithms::unique_from<arr>::value;
  (void)templa::algorithms::reverse_from<arr>::value;

  return 0;
}