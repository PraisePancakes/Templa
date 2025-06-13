#include <iostream>
#include "test_config/definitions.hpp"

int main(int argc, char **argv)
{

  constexpr auto v = templa::index_at_type_v<char, char, int, bool>;

  static_assert(v == 0);

#if TEMPLA_TEST_INSTANCE
  using namespace templa;
  using namespace templa::algorithms;
  using namespace templa::concepts;
  using namespace templa::convert;
  using namespace templa::internal;
  using namespace templa::type_info;
  using namespace templa::traits;

#if TEMPLA_TEST_TYPE_LIST
  {
    using t1 = std::tuple<int, char, long>;
    using t2 = std::tuple<short, bool>;

    using t3 = type_list_append<t1, t2>::type;
    using t7 = type_list_append<t1, char>::type;
    using t4 = type_list_pop_front<int, char>::type;
    using t5 = type_list_pop_front<t1>::type;
    using t6 = type_list_prepend<int, char, short>::type;
    using t8 = type_list_prepend<t1, t2>::type;
    using t9 = type_list_prepend<t1, bool>::type;
    using t10 = type_list_pop_back<int, char, bool>::type;
    using t11 = type_list_pop_back<t1>::type;
    using t12 = type_list_reverse<int, char, long>::type;
    using t13 = type_list_reverse<t1>::type;
    static_assert(index_at_type<int, char, int, bool>::index == 1);
    static_assert(index_at_type<int, t1>::index == 0);
    static_assert(index_at_type_v<short, t2> == 0);
    static_assert(std::is_same_v<int, type_at_index_t<0, t1>>);
    static_assert(templa::index_at_type<int, char, int>::index == 1);
    static_assert(templa::index_at_type_v<int, int, int> == 0);
    static_assert(!std::is_same_v<int, type_at_index_t<1, t1>>);
    static_assert(templa::type_list_contains<int, char, int>::value);
  }
#endif
#if TEMPLA_TEST_CTTI
  {
    static_assert(type_info::ctti::hash_t<int>::id != type_info::ctti::hash_t<char>::id);

    // autological
    static_assert(type_info::ctti::hash_t<int>::id == type_info::ctti::hash_t<int>::id);
  }
#endif
#if TEMPLA_TEST_UNIQUE
  {
    constexpr auto unique_list = algorithms::unique<1, 1, 2, 1, 3, 3>::unique_sequence;
    static_assert(unique_list[0] == 1);
    static_assert(unique_list[1] == 2);
    static_assert(unique_list[2] == 3);
    static_assert(unique_list.size() == 3);

    constexpr std::array<int, 5> array{1, 2, 2, 1, 3};

    constexpr auto unique_list2 = algorithms::unique_from<array>::unique_sequence;
    static_assert(unique_list2[0] == 1);
    static_assert(unique_list2[1] == 2);
    static_assert(unique_list2[2] == 3);
    static_assert(unique_list2.size() == 3);
  }
#endif

#if TEMPLA_TEST_CONCAT
  {
    constexpr std::array<int, 2>
        lhs{1, 2};
    constexpr std::array<int, 3> rhs{1, 2, 3};

    constexpr auto narr = algorithms::concat(lhs, rhs);

    static_assert(narr[0] == 1);
    static_assert(narr[1] == 2);
    static_assert(narr[2] == 1);
    static_assert(narr[3] == 2);
    static_assert(narr[4] == 3);
  }
#endif

#if TEMPLA_TEST_JOIN
  {
    constexpr static std::string_view str1 = "hi";
    constexpr static std::string_view str2 = "bob";

    constexpr static std::string_view s = algorithms::join_v<str1, str2>;
    static_assert(s == "hibob");
  }
#endif

#if TEMPLA_TEST_MAX
  {
    struct A
    {
      int val;
      constexpr A() : val{} {}
      constexpr A(int v) : val{v} {}

      constexpr bool operator==(const A &o) const { return this->val == o.val; }
      constexpr bool operator!=(const A &o) const { return !(this->val == o.val); }
      constexpr bool operator<(const A &o) const { return this->val < o.val; }
      constexpr bool operator>(const A &o) const { return this->val > o.val; }
      constexpr bool operator<=(const A &o) const { return this->val <= o.val; }
      constexpr bool operator>=(const A &o) const { return this->val >= o.val; }

      constexpr int operator*() const { return val; }
      ~A() = default;
    };

    constexpr A a{1};
    constexpr A b{2};
    constexpr A c{3};

    constexpr std::array<A, 3> arr{a, b, c};

    static_assert(templa::algorithms::max_from<arr>::value == c);
  }
#endif

#if TEMPLA_TEST_MIN
  {

    struct A
    {
      int val;
      constexpr A() : val{} {}
      constexpr A(int v) : val{v} {}

      constexpr bool operator==(const A &o) const { return this->val == o.val; }
      constexpr bool operator!=(const A &o) const { return !(this->val == o.val); }
      constexpr bool operator<(const A &o) const { return this->val < o.val; }
      constexpr bool operator>(const A &o) const { return this->val > o.val; }
      constexpr bool operator<=(const A &o) const { return this->val <= o.val; }
      constexpr bool operator>=(const A &o) const { return this->val >= o.val; }

      constexpr int operator*() const { return val; }
      ~A() = default;
    };

    constexpr A a{1};
    constexpr A b{2};
    constexpr A c{3};

    constexpr std::array<A, 3> arr{a, b, c};

    static_assert(templa::algorithms::min<1, 2, 3>::value == 1);
    static_assert(templa::algorithms::min<a, b, c>::value == a);
    static_assert(templa::algorithms::min_from<arr>::value == a);
  }
#endif

#if TEMPLA_TEST_REVERSE
  {
    constexpr auto reverse_list = algorithms::reverse<1, 1, 2, 1, 3, 3>::reverse_sequence;
    static_assert(reverse_list[0] == 3);
    static_assert(reverse_list[1] == 3);
    static_assert(reverse_list[2] == 1);
    static_assert(reverse_list[3] == 2);
    static_assert(reverse_list[4] == 1);
    static_assert(reverse_list[5] == 1);
    static_assert(reverse_list.size() == 6);

    constexpr std::array<int, 5> arr{0, 1, 2, 3, 4};
    constexpr auto reverse_from_list = algorithms::reverse_from<arr>::reverse_sequence;

    static_assert(reverse_from_list[0] == 4);
    static_assert(reverse_from_list[1] == 3);
    static_assert(reverse_from_list[2] == 2);
    static_assert(reverse_from_list[3] == 1);
    static_assert(reverse_from_list[4] == 0);

    static_assert(reverse_from_list.size() == 5);
  }
#endif

#endif // TEMPLA_TEST_INSTANCE
  return 0;
}