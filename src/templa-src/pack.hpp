#pragma once

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
}