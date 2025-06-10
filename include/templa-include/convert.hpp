#pragma once
#include <iostream>
#include <variant>

namespace templa
{
    namespace convert
    {
        template <typename... Ts>
        struct convert_to_variant : std::type_identity<std::variant<Ts...>>
        {
        };

        template <template <typename...> class From, typename... Ts>
        struct convert_to_variant<From<Ts...>> : std::type_identity<std::variant<Ts...>>
        {
        };

        template <typename... Ts>
        struct convert_to_tuple : std::type_identity<std::tuple<Ts...>>
        {
        };

        template <template <typename...> class From, typename... Ts>
        struct convert_to_tuple<From<Ts...>> : std::type_identity<std::tuple<Ts...>>
        {
        };
    }
}