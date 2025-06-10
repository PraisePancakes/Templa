#pragma once
#include "concepts.hpp"
#include <iostream>

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

        template <typename T, std::size_t N, T... elems>
        struct forward_elements
        {
            constexpr static std::array<T, N> array = {elems...};
            constexpr static std::size_t size = N;
            using type = T;
        };

        template <auto a>
            requires(concepts::Container<std::remove_cv_t<decltype(a)>>)
        struct forward_elements_from
        {
            using type = decltype(a)::value_type;

            constexpr static auto value = []<std::size_t... I>(std::index_sequence<I...>) consteval
            {
                return decltype(a){a[I]...};
            }(std::make_index_sequence<a.size()>{});
            constexpr static std::size_t size = value.size();
        };

        template <auto... elems>
        struct uniform_element_identity
        {
        public:
            constexpr static std::size_t size = sizeof...(elems);
            constexpr static bool valid = (std::is_same_v<std::decay_t<decltype(elems)>, std::decay_t<decltype(std::get<0>(std::tuple{elems...}))>> && ...);
            static_assert(valid && (concepts::Comparable<decltype(elems)> && ...),
                          "elements must be of uniform type");

        private:
            constexpr static auto lambda =
                []<std::size_t... I>(std::index_sequence<I...>)
            {
                constexpr std::tuple<decltype(elems)...> tup{elems...};
                return std::get<0>(tup);
            };

        public:
            using uniform_type = typename std::array<decltype(lambda(std::make_index_sequence<size>{})), size>;
            using value_type = typename uniform_type::value_type;
            constexpr static uniform_type identity_value{elems...};
        };

    }
}