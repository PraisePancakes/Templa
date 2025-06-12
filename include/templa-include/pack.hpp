#pragma once
#include "concepts.hpp"
#include <iostream>
/** \defgroup pack
 *
 */

/** \file
 *  \brief Internal utilities and implementation details.
 *  \ingroup pack
 */
namespace templa::internal
{
    /**
     * @brief Internal utilities and implementation details.
     */

    /**
     * @brief Hidden internal types used for template metaprogramming.
     */
    namespace hidden
    {
        /**
         * @brief A template pack to hold a variadic list of types.
         * @tparam Ts Variadic template parameter pack of types.
         */
        template <typename... Ts>
        struct pack;

        /**
         * @brief A template pack holding a list of values of type T.
         * @tparam T The type of elements.
         * @tparam elems Non-type template parameter pack of elements.
         */
        template <typename T, T... elems>
        struct element_pack;
    }

    /**
     * @brief A compile-time list of types.
     *
     * @tparam Ts Variadic template parameter pack of types.
     */
    template <typename... Ts>
    struct type_list
    {
        /// Type alias for the internal pack of types.
        using type = hidden::pack<Ts...>;

        /// Number of types in the list.
        constexpr static size_t size = sizeof...(Ts);
    };

    /**
     * @brief Specialization for wrapping a template class parameter pack into a type_list.
     *
     * @tparam T Template class template parameter.
     * @tparam Ts Template parameters of T.
     */
    template <template <typename...> class T, typename... Ts>
    struct type_list<T<Ts...>>
    {
        /// Type alias for the type_list containing the unpacked types.
        using type = type_list<Ts...>;
    };

    /**
     * @brief Forward a list of elements as a std::array.
     *
     * @tparam T Type of the elements.
     * @tparam N Number of elements.
     * @tparam elems Non-type template parameter pack of elements.
     */
    template <typename T, std::size_t N, T... elems>
    struct forward_elements
    {
        /// Static array holding the elements.
        constexpr static std::array<T, N> array = {elems...};

        /// Number of elements.
        constexpr static std::size_t size = N;

        /// The type of elements.
        using type = T;
    };

    /**
     * @brief Extract elements from a container-like template parameter as a new container.
     *
     * @tparam a A container-like non-type template parameter.
     */
    template <auto a>
        requires(concepts::Container<std::remove_cv_t<decltype(a)>>)
    struct forward_elements_from
    {
        /// Value type of the container.
        using type = decltype(a)::value_type;

        /**
         * @brief Compile-time generated container with the elements of `a`.
         * Uses index_sequence to access each element.
         */
        constexpr static auto value = []<std::size_t... I>(std::index_sequence<I...>) consteval
        {
            return decltype(a){a[I]...};
        }(std::make_index_sequence<a.size()>{});

        /// Number of elements in the generated container.
        constexpr static std::size_t size = value.size();
    };

    /**
     * @brief Uniform element identity helper for a list of values.
     *
     * Enforces all elements have the same type and are comparable.
     *
     * @tparam elems Non-type template parameter pack of elements.
     */
    template <auto... elems>
    struct uniform_element_identity
    {
    public:
        /// Number of elements.
        constexpr static std::size_t size = sizeof...(elems);

        /// Check if all elements have the same decayed type and are comparable.
        constexpr static bool valid = (std::is_same_v<std::decay_t<decltype(elems)>, std::decay_t<decltype(std::get<0>(std::tuple{elems...}))>> && ...) &&
                                      (concepts::Comparable<decltype(elems)> && ...);

        static_assert(valid,
                      "elements must be of uniform type and comparable");

    private:
        /// Helper lambda to deduce uniform element type.
        constexpr static auto lambda =
            []<std::size_t... I>(std::index_sequence<I...>)
        {
            constexpr std::tuple<decltype(elems)...> tup{elems...};
            return std::get<0>(tup);
        };

    public:
        /// Type alias for the uniform std::array of elements.
        using uniform_type = typename std::array<decltype(lambda(std::make_index_sequence<size>{})), size>;

        /// Value type of the uniform_type.
        using value_type = typename uniform_type::value_type;

        /// Static array holding the elements.
        constexpr static uniform_type identity_value{elems...};
    };

} // namespace templa::internal
