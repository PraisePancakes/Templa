#pragma once
#include <iostream>
#include <tuple>
#include <variant>

namespace templa
{

    namespace internal
    {
        namespace hidden
        {
            template <typename... Ts>
            struct pack;
        }

        template <typename... Ts>
        struct type_list
        {
            using type = hidden::pack<Ts...>;
            constexpr static size_t size = sizeof...(Ts);
        };

        template <template <typename...> class T, typename... Ts>
        struct type_list<T<Ts...>>
        {
            using type = type_list<Ts...>;
        };

    }

    namespace convert
    {
        template <typename... Ts>
        struct convert_to_variant
        {
            using type = std::variant<Ts...>;
        };

        template <template <typename...> class From, typename... Ts>
        struct convert_to_variant<From<Ts...>>
        {
            using type = std::variant<Ts...>;
        };

        template <typename... Ts>
        struct convert_to_tuple
        {
            using type = std::tuple<Ts...>;
        };

        template <template <typename...> class From, typename... Ts>
        struct convert_to_tuple<From<Ts...>>
        {
            using type = std::tuple<Ts...>;
        };
    }

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

    template <typename T, typename... List>
    struct type_list_index_from_type;

    template <typename T, typename... List>
    struct type_list_index_from_type<T, T, List...>
    {
        constexpr static size_t index = 0;
    };

    template <typename T, typename U, typename... List>
    struct type_list_index_from_type<T, U, List...>
    {
        constexpr static size_t index = 1 + type_list_index_from_type<T, List...>::index;
    };

    template <typename T, template <typename...> class U, typename... Ts>
    struct type_list_index_from_type<T, U<Ts...>>
    {
        constexpr static size_t index = type_list_index_from_type<T, Ts...>::index;
    };

    template <std::size_t N, typename... List>
    struct type_list_type_from_index
    {
        using type_at_index = std::tuple_element<N, typename internal::type_list<List...>::type>::type;
    };

    template <std::size_t N, template <typename...> class TList, typename... Ts>
    struct type_list_type_from_index<N, TList<Ts...>>
    {
        using tuple_t = convert::convert_to_tuple<internal::type_list<Ts...>>::type;
        using type_at_index = std::tuple_element<N, tuple_t>::type;
    };

    namespace ctti
    {
        template <typename C>
        struct hash_t
        {
        private:
            constexpr static int _var{0};

        public:
            constexpr static auto id{&_var};
        };
    }

};