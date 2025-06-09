#include <iostream>
#include "src/templa.hpp"
#include "tests/tests.hpp"
#include <algorithm>

int main(int argc, char **argv)
{
  templa::test::test_min();
  templa::test::test_unique();
  templa::test::test_concat();
  templa::test::test_join();
  templa::test::test_min();
  templa::test::test_reverse();

  return 0;
}