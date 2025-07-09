#pragma once
#include <iostream>
#include <tuple>
/** \defgroup type_list
 *
 */

/** \file
 *  \brief Provides utilities for compile time type list manipulation and operations.
 *  \ingroup type_list
 */
namespace templa {
/**
 * @brief Appends types or type lists to an existing type list.
 *
 * @tparam Initial Initial types or a type list to append to.
 * @tparam Ts Types to append.
 */
template <typename Initial, typename... Ts>
struct type_list_append : internal::type_list<Initial, Ts...> {
};

/**
 * @brief Specialization to append one type list to another.
 *
 * @tparam Initial Template class for the initial type list.
 * @tparam Appender Template class for the appended type list.
 * @tparam Inits Types in the initial list.
 * @tparam Apps Types to append.
 */
template <template <typename...> class Initial, template <typename...> class Appender, typename... Inits, typename... Apps>
struct type_list_append<Initial<Inits...>, Appender<Apps...>> : internal::type_list<Inits..., Apps...> {
};

/**
 * @brief Append a single type to a type list.
 *
 * @tparam Initial Template class for the initial type list.
 * @tparam Elem Single element type to append.
 * @tparam Inits Types in the initial list.
 */
template <template <typename...> class Initial, typename Elem, typename... Inits>
struct type_list_append<Initial<Inits...>, Elem> : internal::type_list<Inits..., Elem> {
};

/**
 * @brief Prepends types or type lists to an existing type list.
 *
 * @tparam Initial Initial types or a type list to prepend to.
 * @tparam Preps Types to prepend.
 */
template <typename Initial, typename... Preps>
struct type_list_prepend : internal::type_list<Preps..., Initial> {
};

/**
 * @brief Specialization to prepend one type list to another.
 *
 * @tparam Initial Template class for the initial type list.
 * @tparam Prepender Template class for the prepended type list.
 * @tparam Inits Types in the initial list.
 * @tparam Preps Types to prepend.
 */
template <template <typename...> class Initial, template <typename...> class Prepender, typename... Inits, typename... Preps>
struct type_list_prepend<Initial<Inits...>, Prepender<Preps...>> : internal::type_list<Preps..., Inits...> {
};

/**
 * @brief Prepend a single type to a type list.
 *
 * @tparam Initial Template class for the initial type list.
 * @tparam Elem Single element type to prepend.
 * @tparam Inits Types in the initial list.
 */
template <template <typename...> class Initial, typename Elem, typename... Inits>
struct type_list_prepend<Initial<Inits...>, Elem> : internal::type_list<Elem, Inits...> {
};

/**
 * @brief Reverse the order of types in a type list.
 *
 * @tparam T Types to reverse.
 */
template <typename... T>
struct type_list_reverse;

/**
 * @brief Reverse a pack of types.
 *
 * @tparam T First type.
 * @tparam U Second type.
 * @tparam Ts Remaining types.
 */
template <typename T, typename U, typename... Ts>
struct type_list_reverse<T, U, Ts...> : internal::type_list<Ts..., U, T> {
};

/**
 * @brief Reverse types in a templated type list.
 *
 * @tparam Tlist Template class containing types.
 * @tparam T First type.
 * @tparam U Second type.
 * @tparam Ts Remaining types.
 */
template <template <typename...> class Tlist, typename T, typename U, typename... Ts>
struct type_list_reverse<Tlist<T, U, Ts...>> : internal::type_list<Ts..., U, T> {
};

/**
 * @brief Remove the first type from a type list.
 *
 * @tparam T First type.
 * @tparam Ts Remaining types.
 */
template <typename... T>
struct type_list_pop_front;

/**
 * @brief Remove the first type from a pack of types.
 *
 * @tparam T First type.
 * @tparam Ts Remaining types.
 */
template <typename T, typename... Ts>
struct type_list_pop_front<T, Ts...> : internal::type_list<Ts...> {
    /// The popped (removed) first type.
    using popped = T;
};

/**
 * @brief Remove the first type from a templated type list.
 *
 * @tparam Tlist Template class containing types.
 * @tparam U First type.
 * @tparam Ts Remaining types.
 */
template <template <typename...> class Tlist, typename U, typename... Ts>
struct type_list_pop_front<Tlist<U, Ts...>> : internal::type_list<Ts...> {
    /// The popped (removed) first type.
    using popped = U;
};

/**
 * @brief Remove the last type from a type list.
 *
 * @tparam Ts Types in the list.
 */
template <typename... Ts>
struct type_list_pop_back;

/**
 * @brief Specialization for exactly two types: last type is popped.
 *
 * @tparam Head First type.
 * @tparam Tail Second (last) type.
 */
template <typename Head, typename Tail>
struct type_list_pop_back<Head, Tail> : internal::type_list<Head> {
    /// The popped (removed) last type.
    using popped = Tail;
};

/**
 * @brief Remove the last type from a longer pack recursively.
 *
 * @tparam Head First type.
 * @tparam Mid Middle type.
 * @tparam Tail Remaining tail types.
 */
template <typename Head, typename Mid, typename... Tail>
struct type_list_pop_back<Head, Mid, Tail...> {
   private:
    using type_list = type_list_prepend<typename type_list_pop_back<Mid, Tail...>::type, Head>;

   public:
    /// Type list without the last type.
    using type = type_list::type;
    /// Number of types after popping.
    constexpr static size_t size = type::size;
    /// The popped (removed) last type.
    using popped = typename type_list_pop_back<Mid, Tail...>::popped;
};

/**
 * @brief Remove the last type from a templated type list.
 *
 * @tparam Tlist Template class containing types.
 * @tparam Ts Types in the list.
 */
template <template <typename...> class Tlist, typename... Ts>
struct type_list_pop_back<Tlist<Ts...>> {
   private:
    using type_list = type_list_pop_back<Ts...>;

   public:
    /// Type list without the last type.
    using type = type_list::type;
    /// Number of types after popping.
    constexpr static size_t size = type::size;
    /// The popped (removed) last type.
    using popped = type_list::popped;
};

namespace internal {
/**
 * @brief Helper visitor struct inheriting from multiple function objects.
 *
 * Used to dispatch calls based on index.
 *
 * @tparam Fns Function objects.
 */
template <typename... Fns>
struct visitor : Fns... {
    using Fns::operator()...;
};
};  // namespace internal

/**
 * @brief Integral constant wrapper for a size_t value.
 *
 * Used as a tag for compile-time indexing.
 *
 * @tparam x The value.
 */
template <std::size_t x>
using value = std::integral_constant<std::size_t, x>;

/**
 * @brief Get the type at a given index in a parameter pack.
 *
 * @tparam idx Index to access.
 * @tparam Ts Parameter pack of types.
 */
template <std::size_t idx, typename... Ts>
struct type_at_index {
    static_assert(idx <= sizeof...(Ts) && idx >= 0, "Index out of bounds");
    /// Type at index `idx`.
    using type = std::tuple_element_t<idx, std::tuple<Ts...>>;
};

/**
 * @brief Get the type at a given index in a templated type list.
 *
 * @tparam idx Index to access.
 * @tparam T Template type list.
 */
template <std::size_t idx, template <typename...> class T, typename... Ts>
struct type_at_index<idx, T<Ts...>> {
    using type = type_at_index<idx, Ts...>::type;
};

/**
 * @brief Convenience alias for type_at_index::type.
 *
 * @tparam idx Index to access.
 * @tparam Ts Parameter pack of types.
 */
template <std::size_t idx, typename... Ts>
using type_at_index_t = type_at_index<idx, Ts...>::type;

/**
 * @brief Check if a type list contains a given type.
 *
 * @tparam T Type to check.
 * @tparam Ts Types to search in.
 */
template <typename T, typename... Ts>
struct type_list_contains {
    /// True if T is in Ts...
    constexpr static bool value = (std::is_same_v<T, Ts> || ...);
};

/**
 * @brief Check if a templated type list contains a given type.
 *
 * @tparam T Type to check.
 * @tparam U Template type list.
 * @tparam Ts Types in the list.
 */
template <typename T, template <typename...> class U, typename... Ts>
struct type_list_contains<T, U<Ts...>> {
    constexpr static bool value = type_list_contains<T, Ts...>::value;
};

/**
 * @brief Get the index of a type in a type list.
 *
 *
 *
 * @tparam T Type to find.
 * @tparam List Types in the list.
 * @return returns -1 if index is not found else returns index (0-indexed value)
 */
template <typename T, typename... List>
struct index_at_type {
    /// Index of type T in List.
    constexpr static auto index = []() {
        return (type_list_contains<T, List...>::value ? []() {signed i = 0;
            (... && (!std::is_same_v<T, List> && ++i));
            return i; }() : -1);
    }();
};

/**
 * @brief Get the index of a type in a templated type list.
 *
 *
 *
 * @tparam T Type to find.
 * @tparam U Template type list.
 * @tparam List Types in the list.
 * @return returns -1 if index is not found else returns index (0-indexed value)
 */
template <typename T, template <typename...> class U, typename... List>
struct index_at_type<T, U<List...>> {
    /// Index of type T in List.
    constexpr static auto index = index_at_type<T, List...>::index;
};

/**
 * @brief Convenience variable for index_at_type::index.
 *
 * @tparam T Type to find.
 * @tparam List Types in the list.
 * @return returns -1 if index is not found else returns index (0-indexed value)
 */
template <typename T, typename... List>
constexpr static auto index_at_type_v = index_at_type<T, List...>::index;

/**
 * \ingroup type_list
 * \brief Recursively flattens nested type_lists into a single flat type_list.
 *
 * Given arbitrarily nested \c type_list<Ts...> structures, this metafunction
 * produces a single-level type list containing all inner types.
 *
 * ### Example
 * \code
 * using t1 = internal::type_list<int, float>;
 * using t2 = internal::type_list<char, t1, double>;
 * using flat = flatten<t2>::type; // type_list<char, int, float, double>
 * \endcode
 */
template <typename... Ts>
struct type_list_flatten;

/**
 * \brief Base case: flattening an empty list yields an empty type_list.
 */
template <>
struct type_list_flatten<> : internal::type_list<> {
};

/**
 * \brief If the input is a non-type_list type, return it as a single-element type_list.
 */
template <typename T>
struct type_list_flatten<T> : internal::type_list<T> {
};

/**
 * \brief If the input is a type_list, recurse into its elements.
 *
 * This case handles flattening nested type_lists by unpacking and re-flattening
 * their individual elements.
 */
template <typename... Ts>
struct type_list_flatten<internal::type_list<Ts...>> : type_list_flatten<Ts...> {
};

/**
 * \brief Recursive case: flatten head and tail separately and append results.
 *
 * Splits the pack into head and tail, flattens each, then merges them
 * using \ref type_list_append.
 */
template <typename T, typename... Rest>
struct type_list_flatten<T, Rest...> {
   private:
    using head = typename type_list_flatten<T>::type;
    using tail = typename type_list_flatten<Rest...>::type;

   public:
    using type = typename type_list_append<head, tail>::type;
};

/**
 * \ingroup type_list
 * \brief Removes duplicate types from a parameter pack.
 *
 * Recursively deduplicates types while preserving the order of first occurrence.
 *
 * ### Example
 * \code
 * using input = type_list<int, float, int, char, float>;
 * using result = type_list_unique<int, float, int, char, float>::type;
 * // result = type_list<int, float, char>
 * \endcode
 */
template <typename... Ts>
struct type_list_unique;

template <>
struct type_list_unique<> : internal::type_list<> {
};

template <typename T, typename... Ts>
struct type_list_unique<T, Ts...> {
   private:
    using tail = typename type_list_unique<Ts...>::type;

   public:
    using type = std::conditional_t<
        type_list_contains<T, tail>::value,
        tail,
        typename type_list_append<internal::type_list<T>, tail>::type>;
};

template <typename... Ts>
using type_list_unique_t = typename type_list_unique<Ts...>::type;

/**
 * @brief Utility to create an offset std::index_sequence.
 *
 * Given a base index sequence and an offset, this struct generates
 * a new index sequence with each index incremented by the offset.
 *
 * @tparam Offset The starting offset to add to each index.
 * @tparam Seq The original std::index_sequence.
 */
template <std::size_t Offset, typename Seq>
struct offset_index_sequence;

/**
 * @brief Specialization for generating an offset index sequence.
 *
 * @tparam Offset The value to add to each index.
 * @tparam Is The original indices.
 */
template <std::size_t Offset, std::size_t... Is>
struct offset_index_sequence<Offset, std::index_sequence<Is...>> {
    /// Resulting index sequence with offset applied.
    using type = std::index_sequence<(Is + Offset)...>;
};

/**
 * @brief Splits a type list into two sublists using index sequences.
 *
 * This template extracts the types at positions specified by two index sequences
 * and places them into two separate template instances.
 *
 * @tparam T The template type to reconstruct the result (e.g., a type list).
 * @tparam F Index sequence for the first half.
 * @tparam S Index sequence for the second half.
 */
template <typename T, typename F, typename S>
struct type_list_split;

/**
 * @brief Specialization of type_list_split for variadic type lists.
 *
 * @tparam T The outer template (e.g., type_list).
 * @tparam Args The types contained in the list.
 * @tparam Is Indices for the first half.
 * @tparam Js Indices for the second half.
 */
template <template <typename...> class T, typename... Args, std::size_t... Is, std::size_t... Js>
struct type_list_split<T<Args...>, std::index_sequence<Is...>, std::index_sequence<Js...>> {
    /// First half of the split type list.
    using first = T<std::tuple_element_t<Is, std::tuple<Args...>>...>;

    /// Second half of the split type list.
    using second = T<std::tuple_element_t<Js, std::tuple<Args...>>...>;
};

/**
 * @brief Splits a type list into two halves.
 *
 * This struct uses index sequences to divide a variadic template list
 * into two halves. The resulting `first` and `second` types contain
 * the two parts.
 *
 * @tparam Ts The full type list to split.
 */
template <typename Ts>
struct type_list_split_half;

/**
 * @brief Specialization to split T<Args...> into two halves.
 *
 * @tparam T The template type (e.g., type_list).
 * @tparam Args The parameter pack of types.
 */
template <template <typename...> class T, typename... Args>
struct type_list_split_half<T<Args...>> {
   private:
    /// Midpoint index for splitting the list.
    static constexpr std::size_t mid = sizeof...(Args) / 2;

    /// Index sequence for the first half.
    using first_indices = std::make_index_sequence<mid>;

    /// Index sequence for the second half (offset from mid).
    using second_indices = typename offset_index_sequence<
        mid,
        std::make_index_sequence<sizeof...(Args) - mid>>::type;

    /// Intermediate struct that performs the actual split.
    using full_type = type_list_split<T<Args...>, first_indices, second_indices>;

   public:
    /// First half of the split type list.
    using first = typename full_type::first;

    /// Second half of the split type list.
    using second = typename full_type::second;
};

/**
 * @brief Intersection of two lists of types.
 *
 * @tparam T The first type list.
 * @tparam U The second type list.
 */

template <typename T, typename U>
struct type_list_intersect;

template <typename... Ts, typename... Us>
struct type_list_intersect<internal::type_list<Ts...>, internal::type_list<Us...>> {
   private:
    using common_types = decltype(std::tuple_cat(
        (std::conditional_t<type_list_contains<Ts, Us...>::value, std::tuple<Ts>, std::tuple<>>{})...));

    template <typename... Tys>
    constexpr static internal::type_list<Tys...> to_type_list(std::tuple<Tys...>);

   public:
    using type = decltype(to_type_list(common_types{}));
};
}  // namespace templa
