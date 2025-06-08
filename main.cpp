#include <iostream>
#include "src/templa.hpp"
#include "tests/tests.hpp"
#include <algorithm>

int main(int argc, char **argv)
{
  templa::test::test_min();

  constexpr auto filled_array = []()
  {
    std::array<int, 5> arr{};
    constexpr int x = 1;
    constexpr int y = 2;

    templa::algorithms::static_for<5>([&]<std::size_t... I>(std::array<int, 5> &a, int x, int y) constexpr
                                      { ((a[I] = static_cast<int>(x * y)), ...); }, arr, x, y);

    return arr;
  }();

  static_assert(filled_array[0] == 2);
  static_assert(filled_array[1] == 2);
  static_assert(filled_array[2] == 2);
  static_assert(filled_array[3] == 2);
  static_assert(filled_array[4] == 2);

  return 0;
}