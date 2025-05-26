#pragma once
#include <iostream>

namespace templa
{

    namespace internal
    {
        template <typename... Ts>
        struct tupler_t
        {
            using type = std::tuple<Ts...>;
        };

    }

    template <typename Initial, typename... Ts>
    struct type_list_append : internal::tupler_t<Initial, Ts...>
    {
    };

    template <template <typename...> class Initial, template <typename...> class Appender, typename... Inits, typename... Apps>
    struct type_list_append<Initial<Inits...>, Appender<Apps...>> : internal::tupler_t<Inits..., Apps...>
    {
    };

    template <template <typename...> class Initial, typename Elem, typename... Inits>
    struct type_list_append<Initial<Inits...>, Elem> : internal::tupler_t<Inits..., Elem>
    {
    };

    template <typename Initial, typename... Preps>
    struct type_list_prepend : internal::tupler_t<Preps..., Initial>
    {
    };

    template <template <typename...> class Initial, template <typename...> class Prepender, typename... Inits, typename... Preps>
    struct type_list_prepend<Initial<Inits...>, Prepender<Preps...>> : internal::tupler_t<Preps..., Inits...>
    {
    };

    template <template <typename...> class Initial, typename Elem, typename... Inits>
    struct type_list_prepend<Initial<Inits...>, Elem> : internal::tupler_t<Elem, Inits...>
    {
    };

    // helper declaration
    template <typename... T>
    struct type_list_pop_front;

    template <typename T, typename... Ts>
    struct type_list_pop_front<T, Ts...> : internal::tupler_t<Ts...>
    {
        using popped = T;
    };

    template <template <typename...> class Tlist, typename U, typename... Ts>
    struct type_list_pop_front<Tlist<U, Ts...>> : internal::tupler_t<Ts...>
    {
        using popped = U;
    };

    template <typename... Ts>
    struct type_list_pop_back;

    template <typename Head, typename Tail>
    struct type_list_pop_back<Head, Tail> : internal::tupler_t<Head>
    {
        using popped = Tail;
    };

    template <typename Head, typename Mid, typename... Tail>
    struct type_list_pop_back<Head, Mid, Tail...>
    {
        using type = type_list_prepend<typename type_list_pop_back<Mid, Tail...>::type, Head>::type;
        using popped = typename type_list_pop_back<Mid, Tail...>::popped;
    };

    template <template <typename...> class Tlist, typename... Ts>
    struct type_list_pop_back<Tlist<Ts...>>
    {
    private:
        using type_list = type_list_pop_back<Ts...>;

    public:
        using type = type_list::type;
        using popped = type_list::popped;
    };

};