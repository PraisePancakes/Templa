#pragma once
#include <iostream>

namespace templa
{
    template <typename Initial, typename... Ts>
    struct type_list_append : internal::type_list<Initial, Ts...>
    {
    };

    template <template <typename...> class Initial, template <typename...> class Appender, typename... Inits, typename... Apps>
    struct type_list_append<Initial<Inits...>, Appender<Apps...>> : internal::type_list<Inits..., Apps...>
    {
    };

    template <template <typename...> class Initial, typename Elem, typename... Inits>
    struct type_list_append<Initial<Inits...>, Elem> : internal::type_list<Inits..., Elem>
    {
    };

    template <typename Initial, typename... Preps>
    struct type_list_prepend : internal::type_list<Preps..., Initial>
    {
    };

    template <template <typename...> class Initial, template <typename...> class Prepender, typename... Inits, typename... Preps>
    struct type_list_prepend<Initial<Inits...>, Prepender<Preps...>> : internal::type_list<Preps..., Inits...>
    {
    };

    template <template <typename...> class Initial, typename Elem, typename... Inits>
    struct type_list_prepend<Initial<Inits...>, Elem> : internal::type_list<Elem, Inits...>
    {
    };

    // helper declaration
    template <typename... T>
    struct type_list_pop_front;

    template <typename T, typename... Ts>
    struct type_list_pop_front<T, Ts...> : internal::type_list<Ts...>
    {
        using popped = T;
    };

    template <template <typename...> class Tlist, typename U, typename... Ts>
    struct type_list_pop_front<Tlist<U, Ts...>> : internal::type_list<Ts...>
    {
        using popped = U;
    };

    template <typename... Ts>
    struct type_list_pop_back;

    template <typename Head, typename Tail>
    struct type_list_pop_back<Head, Tail> : internal::type_list<Head>
    {
        using popped = Tail;
    };

    template <typename Head, typename Mid, typename... Tail>
    struct type_list_pop_back<Head, Mid, Tail...>
    {

    private:
        using type_list = type_list_prepend<typename type_list_pop_back<Mid, Tail...>::type, Head>;

    public:
        using type = type_list::type;
        constexpr static size_t size = type::size;
        using popped = typename type_list_pop_back<Mid, Tail...>::popped;
    };

    template <template <typename...> class Tlist, typename... Ts>
    struct type_list_pop_back<Tlist<Ts...>>
    {
    private:
        using type_list = type_list_pop_back<Ts...>;

    public:
        using type = type_list::type;
        constexpr static size_t size = type::size;
        using popped = type_list::popped;
    };

    namespace internal
    {

        template <typename... Fns>
        struct visitor : Fns...
        {
            using Fns::operator()...;
        };
    };

    template <std::size_t x>
    using value = std::integral_constant<std::size_t, x>;

    template <std::size_t idx, typename... Ts>
    struct type_at_index
    {
        static_assert(idx <= sizeof...(Ts) && idx >= 0);

        using type = decltype([]<std::size_t... i>(std::index_sequence<i...>)
                              { return internal::visitor{
                                    [](value<i>)
                                    {
                                        return std::type_identity<Ts>();
                                    }...}(value<idx>{}); }(std::index_sequence_for<Ts...>()))::type;
    };

    template <std::size_t idx, template <typename...> class T, typename... Ts>
    struct type_at_index<idx, T<Ts...>>
    {
        using type = type_at_index<idx, Ts...>::type;
    };

    template <std::size_t idx, typename... Ts>
    using type_at_index_t = type_at_index<idx, Ts...>::type;

    template <typename T, typename... List>
    struct index_at_type
    {
        constexpr static std::size_t index = []()
        {
            std::size_t i = 0;
            (... && (!std::is_same_v<T, List> && i++));
            return i;
        }();
    };

    template <typename T, template <typename...> class U, typename... List>
    struct index_at_type<T, U<List...>>
    {
        constexpr static std::size_t index = index_at_type<T, List...>::index;
    };

    template <typename T, typename... List>
    constexpr static std::size_t index_at_type_v = index_at_type<T, List...>::index;
};