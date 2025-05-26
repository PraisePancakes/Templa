#include <iostream>
#include "src/templa.hpp"

int main(int argc, char **argv)
{
    using namespace templa;

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
    static_assert(type_list_index_of<int, char, int, bool>::index == 1);
    static_assert(type_list_index_of<int, t1>::index == 0);
    static_assert(type_list_index_of<short, t2>::index == 0);
    static_assert(ctti::hash_t<int>::id != ctti::hash_t<char>::id);

    return 0;
}