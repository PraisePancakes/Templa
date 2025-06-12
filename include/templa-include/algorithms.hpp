#pragma once
#include <array>
#include <iostream>
#include <vector>
#include <set>
#include "concepts.hpp"
#include "pack.hpp"
#include <algorithm>
#include "static_for.hpp"

/** \defgroup algorithms
 *  Utilities for compile-time operations.
 */

/** \file
 *  \brief Algorithms
 *  \ingroup algorithms
 */
namespace templa::algorithms
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

    /**
     * @brief Forwards array along with index sequence of the array size to the implementation.
     *
     * @tparam T        The element type of the input std::array.
     * @tparam Formats  A parameter pack representing the target types for each tuple element.
     * @tparam N        The size of the input array.
     *
     * @param arr       The input std::array of type T with N elements.
     */

    template <typename T, typename... Formats, std::size_t N>
    std::tuple<Formats...> as_tuple(std::array<T, N> const &arr)
    {
        return as_tuple<Formats...>(arr, std::make_index_sequence<N>{});
    };

    /**
     * @brief Zips two tuples into a single tuple of pairs.
     *
     * Combines two input tuples element-wise into a tuple of std::pairs,
     * where each pair contains elements from the same index in each input tuple.
     *
     * @tparam T1 The type of the first tuple.
     * @tparam T2 The type of the second tuple.
     * @tparam I  A parameter pack of indices used to extract elements from the tuples.
     *
     * @param t1 The first input tuple.
     * @param t2 The second input tuple.
     * @param indices An index sequence used for unpacking tuple elements.
     *
     * @return A tuple of std::pairs, each containing one element from t1 and one from t2.
     */

    template <typename T1, typename T2, std::size_t... I>
    constexpr static auto zip(T1 const &t1, T2 const &t2, std::index_sequence<I...>)
    {
        return std::make_tuple((std::pair{std::get<I>(t1), std::get<I>(t2)})...);
    };

    /**
     * @brief Zips two tuples into a tuple of pairs.
     *
     * This overload automatically generates the index sequence for zipping two tuples.
     * It requires that the input tuples have the same number of elements.
     *
     * @tparam Ts Parameter pack representing the types of elements in the first tuple.
     * @tparam Us Parameter pack representing the types of elements in the second tuple.
     *
     * @param t1 The first tuple.
     * @param t2 The second tuple.
     *
     * @return A tuple of std::pairs, where each pair contains corresponding elements
     *         from the input tuples.
     *
     * @note The function requires that both tuples have the same number of elements.
     */

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
                if (!internal::exists_until(arr, arr[i], i))
                    cnt++;
            }
            return cnt;
        };
    };

    /**
     * @brief Computes the minimum value among a pack of compile-time constants.
     *
     * This struct accepts a parameter pack of compile-time constants and computes
     * the minimum value using `std::min_element`.
     *
     * @tparam Es The pack of compile-time constant values.
     */
    template <auto... Es>
    struct min : templa::internal::uniform_element_identity<Es...>
    {
        /// Alias for the identity type that represents the uniform compile-time values.
        using identity_type = typename templa::internal::uniform_element_identity<Es...>;

        /// The value type of the elements in the pack.
        using typename identity_type::value_type;

        /// The minimum value among the compile-time constants.
        constexpr static value_type value =
            *std::min_element(identity_type::identity_value.begin(), identity_type::identity_value.end());
    };

    /**
     * @brief Computes the minimum value from a container-like object at compile time.
     *
     * Accepts a container-like expression (e.g., `std::array`, `std::vector` constexpr)
     * and computes the minimum element value at compile-time by unpacking it.
     *
     * @tparam e A container expression with random access and known size.
     * @requires The container must satisfy the `concepts::Container` constraint.
     */
    template <auto e>
        requires(concepts::Container<std::remove_cv_t<decltype(e)>>)
    struct min_from
    {
    public:
        /// The type of the elements in the container.
        using type = typename decltype(e)::value_type;

        /// The size of the container.
        constexpr static std::size_t size = e.size();

        /**
         * @brief The minimum value computed at compile time from the container.
         *
         * Uses the `min` struct to evaluate the minimum by unpacking the container.
         */
        constexpr static auto value =
            []<std::size_t... I>(std::index_sequence<I...>) consteval noexcept
        {
            return min<e[I]...>::value;
        }(std::make_index_sequence<size>{});
    };

    /**
     * @brief Computes the maximum value among a pack of compile-time constants.
     *
     * Inherits from `templa::internal::uniform_element_identity` to extract and store
     * the values in a uniform container, then determines the maximum using
     * `std::max_element`.
     *
     * @tparam Es The pack of compile-time constant values.
     */
    template <auto... Es>
    struct max : templa::internal::uniform_element_identity<Es...>
    {
        /// The type that provides the uniform container of values.
        using identity_type = typename templa::internal::uniform_element_identity<Es...>;

        /// The value type of the elements.
        using typename identity_type::value_type;

        /// The maximum value among the compile-time constants.
        constexpr static value_type value =
            *std::max_element(identity_type::identity_value.begin(), identity_type::identity_value.end(),
                              [](const value_type &a, const value_type &b)
                              { return a < b; });
    };

    /**
     * @brief Computes the maximum value from a container-like object at compile time.
     *
     * Unpacks a constexpr container (e.g., `std::array`) and computes the maximum element
     * using the `max` struct.
     *
     * @tparam e A container expression with known size and random access.
     * @requires The container must satisfy the `concepts::Container` constraint.
     */
    template <auto e>
        requires(concepts::Container<std::remove_cv_t<decltype(e)>>)
    struct max_from
    {
    public:
        /// The type of the elements in the container.
        using type = typename decltype(e)::value_type;

        /// The number of elements in the container.
        constexpr static std::size_t size = e.size();

        /**
         * @brief The maximum value computed at compile time from the container.
         *
         * Internally uses the `max` struct to evaluate the maximum by unpacking the elements.
         */
        constexpr static auto value =
            []<std::size_t... I>(std::index_sequence<I...>) consteval noexcept
        {
            return max<e[I]...>::value;
        }(std::make_index_sequence<size>{});
    };
    /**
     * @brief Computes a compile-time unique sequence from a pack of values.
     *
     * Inherits from `templa::internal::uniform_element_identity` to obtain a uniform
     * array representation of the input pack and eliminates duplicates in the order
     * they appear.
     *
     * @tparam Es A parameter pack of compile-time constant values.
     */
    template <auto... Es>
    struct unique : templa::internal::uniform_element_identity<Es...>
    {
        /// Type containing the uniform array of input values.
        using identity_type = typename templa::internal::uniform_element_identity<Es...>;

        /// The type of the original uniform array.
        using old_array_type = typename identity_type::uniform_type;

    public:
        /**
         * @brief The resulting array with unique elements from the input pack.
         *
         * The algorithm preserves the order of first occurrences and excludes duplicates
         * using `exists_until` from the internal utilities.
         */
        constexpr static auto unique_sequence = []() consteval
        {
            constexpr std::size_t n_unique = internal::count_unique(identity_type::identity_value);
            std::array<typename identity_type::value_type, n_unique> new_arr{};
            std::size_t idx = 0;
            constexpr auto lam = []<std::size_t... I>(
                                     std::array<typename identity_type::value_type, n_unique> &n,
                                     std::size_t &idx)
            {
                constexpr auto &old = identity_type::identity_value;
                ((!internal::exists_until(old, old[I], I) ? (n[idx++] = old[I], void()) : void()), ...);
            };
            static_for<identity_type::size>(lam, new_arr, idx);
            return new_arr;
        }();
    };

    /**
     * @brief Computes a compile-time unique sequence from a container expression.
     *
     * Accepts a `constexpr` container and eliminates duplicate elements using
     * the same logic as the `unique` struct. The container must be both a `Container`
     * and `Comparable`.
     *
     * @tparam a A container expression (e.g., `std::array`) with known contents.
     * @requires The container must satisfy `concepts::Container` and `concepts::Comparable`.
     */
    template <auto a>
        requires(concepts::Container<std::remove_cv_t<decltype(a)>>) &&
                (concepts::Comparable<std::remove_cv_t<decltype(a)>>)
    struct unique_from
    {
    private:
        /// Internal forwarding helper to extract element type and size.
        using forwarded_type = templa::internal::forward_elements_from<a>;

        /// The original container as a standard array.
        using old_array_type = std::array<typename forwarded_type::type, forwarded_type::size>;

        /// The unpacked value of the container.
        constexpr static auto old = templa::internal::forward_elements_from<a>::value;

    public:
        /**
         * @brief The resulting array with unique elements from the input container.
         *
         * Uses an index-based iteration with compile-time filtering to populate
         * a new array containing only the first occurrences of each element.
         */
        constexpr static auto unique_sequence = []() consteval
        {
            std::array<typename forwarded_type::type, internal::count_unique(old)> new_arr{};
            std::size_t idx = 0;
            constexpr auto lam = []<std::size_t... I>(
                                     const old_array_type &o,
                                     std::array<typename forwarded_type::type, internal::count_unique(old)> &n,
                                     std::size_t &idx)
            {
                ((!internal::exists_until(old, old[I], I) ? (n[idx++] = old[I], void()) : void()), ...);
            };
            static_for<old.size()>(lam, old, new_arr, idx);
            return new_arr;
        }();
    };

    /**
     * @brief Reverses a compile-time pack of constant values.
     *
     * Inherits from `templa::internal::uniform_element_identity` to treat the pack
     * as a uniform array, then computes a reversed version of the array at compile time.
     *
     * @tparam elems A parameter pack of compile-time constant values.
     */
    template <auto... elems>
    struct reverse : templa::internal::uniform_element_identity<elems...>
    {
        /// The identity type wrapping the parameter pack.
        using identity_type = typename templa::internal::uniform_element_identity<elems...>;

        /// The array type representing the uniform values.
        using array_type = typename identity_type::uniform_type;

        /**
         * @brief The reversed array, computed at compile time.
         *
         * Constructs a new array where the order of elements is reversed relative to
         * the input pack.
         */
        constexpr static auto reverse_sequence = []()
        {
            auto lam = []<std::size_t... I>(const array_type &in, array_type &out) constexpr
            {
                ((out[I] = in[in.size() - I - 1]), ...);
            };

            std::array<int, identity_type::size> ret{};
            static_for<identity_type::size>(lam, identity_type::identity_value, ret);
            return ret;
        }();
    };

    /**
     * @brief Reverses the elements of a `constexpr` container at compile time.
     *
     * Accepts a container expression (e.g., `std::array`) and constructs a new one
     * with the elements in reversed order.
     *
     * @tparam a A container expression with fixed size and random access.
     * @requires The container must satisfy the `concepts::Container` constraint.
     */
    template <auto a>
        requires(concepts::Container<std::remove_cv_t<decltype(a)>>)
    struct reverse_from
    {
    private:
        /// The original container unpacked to a constexpr array.
        constexpr static auto old = templa::internal::forward_elements_from<a>::value;

    public:
        /**
         * @brief The reversed version of the input container.
         *
         * Constructs a new container by indexing from the end of the original container.
         */
        constexpr static auto reverse_sequence = []<std::size_t... I>(std::index_sequence<I...>)
        {
            constexpr decltype(a) ret{old[a.size() - I - 1]...};
            return ret;
        }(std::make_index_sequence<a.size()>{});
    };

    /**
     * @brief Concatenates two `std::array`s at compile time.
     *
     * Merges the contents of two fixed-size arrays into a single array by invoking
     * an internal helper (`internal::concat_impl`) with appropriate index sequences.
     *
     * @tparam T  The type of the elements in the arrays.
     * @tparam N  The size of the first array (`lhs`).
     * @tparam M  The size of the second array (`rhs`).
     * @param lhs The left-hand array.
     * @param rhs The right-hand array.
     * @return A new `std::array<T, N + M>` containing all elements from `lhs` followed by `rhs`.
     *
     * @see internal::concat_impl
     */
    template <typename T, std::size_t N, std::size_t M>
    constexpr static auto concat(const std::array<T, N> &lhs, const std::array<T, M> &rhs)
    {
        return internal::concat_impl(lhs, rhs, std::make_index_sequence<N>{}, std::make_index_sequence<M>{});
    };

    /**
     * @brief Compile-time string concatenation of multiple `std::string_view` references.
     *
     * Joins multiple string literals (passed as `std::string_view` references) into
     * a single compile-time `std::string_view`.
     *
     * @tparam Strs Parameter pack of `std::string_view` references to be joined.
     *
     * @note This struct is useful for generating compile-time string identifiers or
     *       type names without runtime overhead.
     */
    template <const std::string_view &...Strs>
    struct join
    {
    private:
        /**
         * @brief Internal compile-time character array holding the joined string.
         *
         * Allocates an array of total character length (+1 for null terminator),
         * then inserts each character of every input string in sequence.
         */
        constexpr static auto arr = []() consteval
        {
            constexpr std::size_t length = (Strs.size() + ... + 0);
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
            return arr;
        }();

    public:
        /**
         * @brief The resulting joined string as a `std::string_view`.
         */
        constexpr static std::string_view value = {arr.data(), arr.size() - 1};
    };

    /**
     * @brief Shorthand variable template to extract joined string from `join`.
     *
     * @tparam Strs Same parameter pack of string views as `join`.
     */
    template <std::string_view const &...Strs>
    constexpr static std::string_view join_v = join<Strs...>::value;

    /**
     * @brief Flattens a single-level `std::vector<T>` into another vector.
     *
     * Copies elements from the input vector to the output vector using `insert`.
     *
     * @tparam T The element type of the input vector.
     * @tparam O The element type of the output vector.
     * @param in The input vector to flatten.
     * @param out The output vector to append to.
     */
    template <typename T, typename O>
    void flatten(const std::vector<T> &in, std::vector<O> &out)
    {
        out.insert(out.end(), in.begin(), in.end());
    };

    /**
     * @brief Flattens a two-level nested `std::vector<std::vector<T>>` into another vector.
     *
     * Recursively flattens each inner vector into the output vector.
     *
     * @tparam T The element type of the innermost vectors.
     * @tparam O The element type of the output vector.
     * @param in The nested input vector to flatten.
     * @param out The output vector to append to.
     */
    template <typename T, typename O>
    void flatten(const std::vector<std::vector<T>> &in, std::vector<O> &out)
    {
        for (const auto &e : in)
        {
            flatten(e, out);
        };
    };

    /**
     * @brief Applies a callable to the result of concatenating multiple tuples.
     *
     * Combines the input tuples using `std::tuple_cat`, then invokes the callable
     * with the elements of the concatenated tuple via `std::apply`.
     *
     * @tparam Callable A function, lambda, or callable object that accepts the
     *         unpacked elements of the concatenated tuple.
     * @tparam Ts Parameter pack of tuple types (e.g., `std::tuple<...>`).
     * @param c The callable to invoke with the unpacked concatenated tuple.
     * @param tuples The input tuples to be concatenated and passed to the callable.
     *
     * @note All input tuples must be compatible with `std::tuple_cat`.
     *
     * @example
     * @code
     * auto t1 = std::make_tuple(1, 'a');
     * auto t2 = std::make_tuple(3.14);
     * apply_to_tuple_cat([](int i, char c, double d) {
     *     std::cout << i << ", " << c << ", " << d << '\n';
     * }, t1, t2);
     * // Output: 1, a, 3.14
     * @endcode
     */
    template <typename Callable, typename... Ts>
    constexpr static void apply_to_tuple_cat(Callable &&c, Ts &&...tuples)
    {
        auto cat = std::tuple_cat<Ts...>(std::forward<Ts>(tuples)...);
        std::apply(std::forward<Callable>(c), std::move(cat));
    };

}