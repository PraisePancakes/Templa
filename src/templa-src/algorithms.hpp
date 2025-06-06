#pragma once
#include <array>
#include <iostream>
#include <vector>
#include <set>
#include "concepts.hpp"

namespace templa
{
    namespace algorithms
    {
        template <typename T, typename... Formats, std::size_t N, std::size_t... Is>
        std::tuple<Formats...> as_tuple(std::array<T, N> const &arr, std::index_sequence<Is...>)
        {
            return std::make_tuple(Formats{arr[Is]}...);
        };

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
                if (!exists_until(arr, arr[i], i - 1))
                    cnt++;
            }
            return cnt;
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
            constexpr static auto value = []<std::size_t... I>(std::index_sequence<I...>) consteval
            {
                return decltype(a){a[I]...};
            }(std::make_index_sequence<a.size()>{});
        };

        template <typename T, std::size_t N, T... elems>
        struct unique
        {
        private:
            constexpr static std::array<T, N> arr = forward_elements<T, N, elems...>::array;
            constexpr static auto impl = [](std::array<T, N> old) consteval
            {
                std::array<T, count_unique(arr)> new_arr{};
                std::size_t idx = 0;
                for (std::size_t i = 0; i < N; i++)
                {
                    if (!exists_until(old, old[i], i))
                    {
                        new_arr[idx++] = old[i];
                    };
                };
                return new_arr;
            };

        public:
            constexpr static auto array = impl(arr);
        };

        template <auto a>
            requires(concepts::Container<std::remove_cv_t<decltype(a)>>)
        struct unique_from
        {
        private:
            constexpr static auto old = forward_elements_from<a>::value;

        public:
            constexpr static auto value = []() consteval
            {
                std::array<typename decltype(a)::value_type, count_unique(old)> new_arr{};
                std::size_t idx = 0;
                for (std::size_t i = 0; i < old.size(); i++)
                {
                    if (!exists_until(old, old[i], i))
                    {
                        new_arr[idx++] = old[i];
                    };
                };
                return new_arr;
            }();
        };

        template <typename T, std::size_t N, T... elems>
        struct reverse
        {
        private:
            constexpr static std::array<T, N> old = forward_elements<T, N, elems...>::array;

        public:
            constexpr static std::array<T, N> array = []<std::size_t... I>(std::index_sequence<I...>)
            {
                constexpr std::array<T, N> ret{old[N - I - 1]...};
                return ret;
            }(std::make_index_sequence<N>{});
        };

        template <auto a>
            requires(concepts::Container<std::remove_cv_t<decltype(a)>>)
        struct reverse_from
        {
        private:
            constexpr static auto old = forward_elements_from<a>::value;

        public:
            constexpr static auto value = []<std::size_t... I>(std::index_sequence<I...>)
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
            constexpr static auto impl()
            {
                constexpr std::size_t length = (Strs.size() + ... + 0);
                std::array<char, length + 1> arr;
                auto Joiner = [i = 0, &arr](const std::string_view &s) mutable
                {
                    for (auto c : s)
                    {
                        arr[i++] = c;
                    }
                };
                (Joiner(Strs), ...);
                arr[length] = 0;
                return arr;
            };

        public:
            constexpr static auto arr = impl();
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