#include <iostream>
#include "src/templa.hpp"
#include "tests/algorithms/test_concat.hpp"
#include "tests/test_type_list.hpp"
#include "tests/test_ctti.hpp"
enum class MyFlags : uint8_t
{
  A = 1 << 0,
  B = 1 << 1,
  C = 1 << 2
};

using namespace templa;

int main(int argc, char **argv)
{
  using namespace templa;

  FlagEnum<MyFlags> flags;
  auto f = flags | MyFlags::B | MyFlags::A;

  std::cout << f.HasFlag(MyFlags::C);
  return 0;
}