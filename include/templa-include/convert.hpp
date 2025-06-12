#pragma once
#include <iostream>
#include <variant>
/** \defgroup converts
 *  Templa type list conversions.
 */

/** \file
 *  \brief Converts
 *  \ingroup converts
 */
namespace templa::convert
{

    /**
     * @brief Converts a pack of types into a std::variant of those types.
     *
     * @tparam Ts Parameter pack of types to convert.
     *
     * Usage:
     * @code
     * using V = convert_to_variant<int, float, double>::type; // std::variant<int, float, double>
     * @endcode
     */
    template <typename... Ts>
    struct convert_to_variant : std::type_identity<std::variant<Ts...>>
    {
    };

    /**
     * @brief Converts a template instantiation of types into a std::variant of those types.
     *
     * This specialization extracts the template parameters of a template class and converts them
     * into a std::variant.
     *
     * @tparam From Template template parameter representing the original template.
     * @tparam Ts Parameter pack of types inside the template instantiation.
     *
     * Usage:
     * @code
     * template<typename... Args>
     * struct MyTemplate {};
     *
     * using V = convert_to_variant<MyTemplate<int, float>>::type; // std::variant<int, float>
     * @endcode
     */
    template <template <typename...> class From, typename... Ts>
    struct convert_to_variant<From<Ts...>> : std::type_identity<std::variant<Ts...>>
    {
    };

    /**
     * @brief Converts a pack of types into a std::tuple of those types.
     *
     * @tparam Ts Parameter pack of types to convert.
     *
     * Usage:
     * @code
     * using T = convert_to_tuple<int, float, double>::type; // std::tuple<int, float, double>
     * @endcode
     */
    template <typename... Ts>
    struct convert_to_tuple : std::type_identity<std::tuple<Ts...>>
    {
    };

    /**
     * @brief Converts a template instantiation of types into a std::tuple of those types.
     *
     * This specialization extracts the template parameters of a template class and converts them
     * into a std::tuple.
     *
     * @tparam From Template template parameter representing the original template.
     * @tparam Ts Parameter pack of types inside the template instantiation.
     *
     * Usage:
     * @code
     * template<typename... Args>
     * struct MyTemplate {};
     *
     * using T = convert_to_tuple<MyTemplate<int, float>>::type; // std::tuple<int, float>
     * @endcode
     */
    template <template <typename...> class From, typename... Ts>
    struct convert_to_tuple<From<Ts...>> : std::type_identity<std::tuple<Ts...>>
    {
    };
}
