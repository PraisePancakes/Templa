#include <iostream>
#include "src/templa.hpp"
#include "tests/test_algorithms.hpp"
#include "tests/test_type_list.hpp"
#include "tests/test_ctti.hpp"

int main(int argc, char **argv)
{
  constexpr std::array<int, 5> arr{0, 0, 1, 2, 1};

  static_assert(templa::algorithms::exists_until(arr, 1, 3));
  static_assert(templa::algorithms::count_unique(arr) == 3);

  constexpr auto a = templa::algorithms::unique<int, 3, 1, 1, 2>::array;

  static_assert(a[0] == 1);
  static_assert(a[1] == 2);
  static_assert(a.size() == 2);

  constexpr auto a1 = templa::algorithms::unique_from<int, arr.size(), arr>::array;

  static_assert(a1[0] == 0);
  static_assert(a1[1] == 1);
  static_assert(a1[2] == 2);

  return 0;
}