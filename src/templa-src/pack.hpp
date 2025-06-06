#pragma once

namespace templa
{

    namespace internal
    {
        namespace hidden
        {
            template <typename... Ts>
            struct pack;

            template <typename T, T... elems>
            struct element_pack;
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

        template <typename T, T... elems>
        struct element_list
        {
            constexpr static std::size_t size = sizeof...(elems);
            using underlying = T;
            using type = hidden::element_pack<T, elems...>;
        };

    }
}