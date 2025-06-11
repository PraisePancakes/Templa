#pragma once
#include <array>
#include <iostream>
#include <vector>
#include <set>
#include "concepts.hpp"
#include "pack.hpp"
#include <algorithm>
#include "static_for.hpp"

namespace templa
{
    namespace algorithms
    {
        /**
         * @brief Converts a fixed-size array into a tuple using specified format types.
         *
         * @tparam T        The element type of the input std::array.
         * @tparam Formats  A parameter pack representing the target types for each tuple element.
         * @tparam N        The size of the input array.
         * @tparam Is       A parameter pack of indices used to access elements in the array.
         *
         * @param arr       The input std::array of type T with N elements.
         * @param           std::index_sequence<Is...> used to unpack array indices at compile time.
         *
         * @return std::tuple<Formats...> A tuple containing elements of the array converted to the specified format types.
         *
         * @note The number of elements in `Formats...` must match the number of indices in `Is...`.
         */
        template <typename T, typename... Formats, std::size_t N, std::size_t... Is>
        std::tuple<Formats...> as_tuple(std::array<T, N> const &arr, std::index_sequence<Is...>)
        {
            return std::make_tuple(Formats{arr[Is]}...);
        }

        template <typename T, typename... Formats, std::size_t N>
        std::tuple<Formats...> as_tuple(std::array<T, N> const &arr)
        {
            return as_tuple<Formats...>(arr, std::make_index_sequence<N>{});
        };

        template <typename T1, typename T2, std::size_t... I>
        constexpr static auto zip(T1 const &t1, T2 const &t2, std::index_sequence<I...>)
        {
            return std::make_tuple((std::pair{std::get<I>(t1), std::get<I>(t2)})...);
        };

        template <typename... Ts, typename... Us>
            requires(sizeof...(Ts) == sizeof...(Us))
        constexpr static auto zip(std::tuple<Ts...> const &t1, std::tuple<Us...> const &t2)
        {
            return zip(t1, t2, std::make_index_sequence<sizeof...(Ts)>{});
        };

        namespace internal
        {
            template <typename T, std::size_t N, std::size_t M, std::size_t... I, std::size_t... J>
            constexpr auto concat_impl(std::array<T, N> const &lhs, std::array<T, M> const &rhs, std::index_sequence<I...>, std::index_sequence<J...>)
            {
                return std::array<T, N + M>{lhs[I]..., rhs[J]...};
            };

        };

        template <typename T, std::size_t N>
        consteval static bool exists(std::array<T, N> const &arr, const T &elem)
        {
            for (std::size_t i = 0; i < N; i++)
            {
                if (arr[i] == elem)
                {
                    return true;
                }
            }
            return false;
        }

        template <typename T, std::size_t N>
        consteval static bool exists_until(std::array<T, N> const &arr, const T &elem, std::size_t until)
        {
            for (std::size_t i = 0; i < until; i++)
            {
                if (arr[i] == elem)
                    return true;
            }
            return false;
        };

        template <typename T, std::size_t N>
        consteval static std::size_t count_unique(std::array<T, N> const &arr)
        {
            std::size_t cnt = 0;
            for (std::size_t i = 0; i < N; i++)
            {
                if (!exists_until(arr, arr[i], i))
                    cnt++;
            }
            return cnt;
        };

        template <auto... Es>
        struct min : templa::internal::uniform_element_identity<Es...>
        {
            using identity_type = typename templa::internal::uniform_element_identity<Es...>;
            using typename identity_type::value_type;
            constexpr static value_type value = *std::min_element(identity_type::identity_value.begin(), identity_type::identity_value.end());
        };

        template <auto e>
            requires(concepts::Container<std::remove_cv_t<decltype(e)>>)
        struct min_from
        {
        public:
            using type = typename decltype(e)::value_type;
            constexpr static std::size_t size = e.size();
            constexpr static auto value =
                []<std::size_t... I>(std::index_sequence<I...>) consteval noexcept
            {
                return min<e[I]...>::value;
            }(std::make_index_sequence<size>{});
        };

        template <auto... Es>
        struct max : templa::internal::uniform_element_identity<Es...>
        {
            using identity_type = typename templa::internal::uniform_element_identity<Es...>;
            using typename identity_type::value_type;
            constexpr static value_type value = *std::max_element(identity_type::identity_value.begin(), identity_type::identity_value.end(),
                                                                  [](const value_type &a, const value_type &b)
                                                                  { return a < b; });
        };

        template <auto e>
            requires(concepts::Container<std::remove_cv_t<decltype(e)>>)
        struct max_from
        {
        public:
            using type = typename decltype(e)::value_type;
            constexpr static std::size_t size = e.size();
            constexpr static auto value =
                []<std::size_t... I>(std::index_sequence<I...>) consteval noexcept
            {
                return max<e[I]...>::value;
            }(std::make_index_sequence<size>{});
        };

        template <auto... Es>
        struct unique : templa::internal::uniform_element_identity<Es...>
        {
            using identity_type = typename templa::internal::uniform_element_identity<Es...>;
            using old_array_type = typename identity_type::uniform_type;

        public:
            constexpr static auto unique_sequence = []() consteval
            {
                constexpr std::size_t n_unique = count_unique(identity_type::identity_value);
                std::array<typename identity_type::value_type, n_unique> new_arr{};
                std::size_t idx = 0;
                constexpr auto lam = []<std::size_t... I>(std::array<typename identity_type::value_type, n_unique> &n, std::size_t &idx)
                {
                    constexpr auto &old = identity_type::identity_value;
                    ((!exists_until(old, old[I], I) ? (n[idx++] = old[I], void()) : void()), ...);
                };
                static_for<identity_type::size>(lam, new_arr, idx);
                return new_arr;
            }();
        };

        template <auto a>
            requires(concepts::Container<std::remove_cv_t<decltype(a)>>) &&
                    (concepts::Comparable<std::remove_cv_t<decltype(a)>>)
        struct unique_from
        {
        private:
            using forwarded_type = templa::internal::forward_elements_from<a>;
            using old_array_type = std::array<typename forwarded_type::type, forwarded_type::size>;
            constexpr static auto old = templa::internal::forward_elements_from<a>::value;

        public:
            constexpr static auto unique_sequence = []() consteval
            {
                std::array<typename forwarded_type::type, count_unique(old)> new_arr{};
                std::size_t idx = 0;
                constexpr auto lam = []<std::size_t... I>(const old_array_type &o,
                                                          std::array<typename forwarded_type::type, count_unique(old)> &n,
                                                          std::size_t &idx)
                {
                    ((!exists_until(old, old[I], I) ? (n[idx++] = old[I], void()) : void()), ...);
                };
                static_for<old.size()>(lam, old, new_arr, idx);
                return new_arr;
            }();
        };

        template <auto... elems>
        struct reverse : templa::internal::uniform_element_identity<elems...>
        {
            using identity_type = typename templa::internal::uniform_element_identity<elems...>;
            using array_type = typename identity_type::uniform_type;

            constexpr static auto reverse_sequence = []()
            { 
                auto lam = []<std::size_t... I>(const array_type &in, array_type &out) constexpr
                 { ((out[I] = in[in.size() - I - 1]), ...); };
            std::array<int, identity_type::size> ret{};
            static_for<identity_type::size>(lam, identity_type::identity_value, ret);
            return ret; }();
        };

        template <auto a>
            requires(concepts::Container<std::remove_cv_t<decltype(a)>>)
        struct reverse_from
        {
        private:
            constexpr static auto old = templa::internal::forward_elements_from<a>::value;

        public:
            constexpr static auto reverse_sequence = []<std::size_t... I>(std::index_sequence<I...>)
            {
                constexpr decltype(a) ret{old[a.size() - I - 1]...};
                return ret;
            }(std::make_index_sequence<a.size()>{});
        };

        template <typename T, std::size_t N, std::size_t M>
        constexpr static auto concat(const std::array<T, N> &lhs, const std::array<T, M> &rhs)
        {
            return internal::concat_impl(lhs, rhs, std::make_index_sequence<N>{}, std::make_index_sequence<M>{});
        };

        template <const std::string_view &...Strs>
        struct join
        {
        private:
            constexpr static auto arr = []() consteval
            { constexpr std::size_t length = (Strs.size() + ... + 0);
                std::array<char, length + 1> arr;
                auto Joiner = [i = 0, &arr](const std::string_view &s) consteval mutable
                {
                    for (auto c : s)
                    {
                        arr[i++] = c;
                    }
                };
                (Joiner(Strs), ...);
                arr[length] = 0;
                return arr; }();

        public:
            constexpr static std::string_view value = {arr.data(), arr.size() - 1};
        };

        template <std::string_view const &...Strs>
        constexpr static std::string_view join_v = join<Strs...>::value;

        template <typename T, typename O>
        void flatten(const std::vector<T> &in, std::vector<O> &out)
        {
            out.insert(out.end(), in.begin(), in.end());
        };

        template <typename T, typename O>
        void flatten(const std::vector<std::vector<T>> &in, std::vector<O> &out)
        {
            for (const auto &e : in)
            {
                flatten(e, out);
            };
        };

        template <typename Callable, typename... Ts>
        constexpr static void apply_to_tuple_cat(Callable &&c, Ts &&...tuples)
        {
            auto cat = std::tuple_cat<Ts...>(std::forward<Ts>(tuples)...);
            std::apply(std::forward<Callable>(c), std::move(cat));
        };

    };

}