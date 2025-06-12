#pragma once
#include <iostream>
/** \defgroup type_list
 *
 */

/** \file
 *  \brief Provides utilities for compile time type list manipulation and operations.
 *  \ingroup type_list
 */
namespace templa
{
    /**
     * @brief Appends types or type lists to an existing type list.
     *
     * @tparam Initial Initial types or a type list to append to.
     * @tparam Ts Types to append.
     */
    template <typename Initial, typename... Ts>
    struct type_list_append : internal::type_list<Initial, Ts...>
    {
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
    struct type_list_append<Initial<Inits...>, Appender<Apps...>> : internal::type_list<Inits..., Apps...>
    {
    };

    /**
     * @brief Append a single type to a type list.
     *
     * @tparam Initial Template class for the initial type list.
     * @tparam Elem Single element type to append.
     * @tparam Inits Types in the initial list.
     */
    template <template <typename...> class Initial, typename Elem, typename... Inits>
    struct type_list_append<Initial<Inits...>, Elem> : internal::type_list<Inits..., Elem>
    {
    };

    /**
     * @brief Prepends types or type lists to an existing type list.
     *
     * @tparam Initial Initial types or a type list to prepend to.
     * @tparam Preps Types to prepend.
     */
    template <typename Initial, typename... Preps>
    struct type_list_prepend : internal::type_list<Preps..., Initial>
    {
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
    struct type_list_prepend<Initial<Inits...>, Prepender<Preps...>> : internal::type_list<Preps..., Inits...>
    {
    };

    /**
     * @brief Prepend a single type to a type list.
     *
     * @tparam Initial Template class for the initial type list.
     * @tparam Elem Single element type to prepend.
     * @tparam Inits Types in the initial list.
     */
    template <template <typename...> class Initial, typename Elem, typename... Inits>
    struct type_list_prepend<Initial<Inits...>, Elem> : internal::type_list<Elem, Inits...>
    {
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
    struct type_list_reverse<T, U, Ts...> : internal::type_list<Ts..., U, T>
    {
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
    struct type_list_reverse<Tlist<T, U, Ts...>> : internal::type_list<Ts..., U, T>
    {
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
    struct type_list_pop_front<T, Ts...> : internal::type_list<Ts...>
    {
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
    struct type_list_pop_front<Tlist<U, Ts...>> : internal::type_list<Ts...>
    {
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
    struct type_list_pop_back<Head, Tail> : internal::type_list<Head>
    {
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
    struct type_list_pop_back<Head, Mid, Tail...>
    {
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
    struct type_list_pop_back<Tlist<Ts...>>
    {
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

    namespace internal
    {
        /**
         * @brief Helper visitor struct inheriting from multiple function objects.
         *
         * Used to dispatch calls based on index.
         *
         * @tparam Fns Function objects.
         */
        template <typename... Fns>
        struct visitor : Fns...
        {
            using Fns::operator()...;
        };
    };

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
    struct type_at_index
    {
        static_assert(idx <= sizeof...(Ts) && idx >= 0, "Index out of bounds");

        /// Type at index `idx`.
        using type = decltype([]<std::size_t... i>(std::index_sequence<i...>)
                              { return internal::visitor{
                                    [](value<i>)
                                    {
                                        return std::type_identity<Ts>();
                                    }...}(value<idx>{}); }(std::index_sequence_for<Ts...>()))::type;
    };

    /**
     * @brief Get the type at a given index in a templated type list.
     *
     * @tparam idx Index to access.
     * @tparam T Template type list.
     */
    template <std::size_t idx, template <typename...> class T, typename... Ts>
    struct type_at_index<idx, T<Ts...>>
    {
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
    struct type_list_contains
    {
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
    struct type_list_contains<T, U<Ts...>>
    {
        constexpr static bool value = type_list_contains<T, Ts...>::value;
    };

    /**
     * @brief Get the index of a type in a type list.
     *
     * Requires the type to be present in the list.
     *
     * @tparam T Type to find.
     * @tparam List Types in the list.
     */
    template <typename T, typename... List>
        requires(type_list_contains<T, List...>::value)
    struct index_at_type
    {
        /// Index of type T in List.
        constexpr static std::size_t index = []()
        {
            std::size_t i = 0;
            (... && (!std::is_same_v<T, List> && i++));
            return i;
        }();
    };

    /**
     * @brief Get the index of a type in a templated type list.
     *
     * Requires the type to be present in the list.
     *
     * @tparam T Type to find.
     * @tparam U Template type list.
     * @tparam List Types in the list.
     */
    template <typename T, template <typename...> class U, typename... List>
        requires(type_list_contains<T, List...>::value)
    struct index_at_type<T, U<List...>>
    {
        /// Index of type T in List.
        constexpr static std::size_t index = index_at_type<T, List...>::index;
    };

    /**
     * @brief Convenience variable for index_at_type::index.
     *
     * @tparam T Type to find.
     * @tparam List Types in the list.
     */
    template <typename T, typename... List>
        requires(type_list_contains<T, List...>::value)
    constexpr static std::size_t index_at_type_v = index_at_type<T, List...>::index;

};
