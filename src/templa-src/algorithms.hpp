#pragma once
#include <array>
#include <iostream>
#include <vector>

namespace templa
{
    namespace algorithms
    {

        template <typename T, std::size_t N, std::size_t M>
        constexpr static auto concat(const std::array<T, N> &lhs, const std::array<T, M> &rhs)
        {
            std::array<T, N + M> new_arr;
            size_t new_index = 0;

            for (size_t i = 0; i < N; i++)
            {
                new_arr[new_index] = lhs[i];
                new_index++;
            }

            for (size_t j = 0; j < M; j++)
            {
                new_arr[new_index] = rhs[j];
                new_index++;
            }

            return new_arr;
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

        template <typename T>
            requires std::input_or_output_iterator<T>
        void reverse(T begin, T end)
        {
            if (begin == end)
                return;
            end--;
            while (begin < end)
            {
                std::swap(*begin, *end);
                begin++;
                end--;
            }
        };

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
            std::apply(std::forward<Callable>(c), cat);
        };

    };

}