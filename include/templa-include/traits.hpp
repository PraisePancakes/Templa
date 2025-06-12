#pragma once
#include <iostream>
#include <functional>
/**
 * @namespace traits
 * @brief Utility templates for type manipulation and traits detection.
 *
 * This namespace provides templates to strip qualifiers and pointers from types,
 * extract return types of callable objects and functions,
 * and check if two callables have the same return type.
 */

/** \defgroup traits
 *
 */

/** \file
 *  \brief Utility templates for type manipulation and traits detection.
 *  \ingroup traits
 */
namespace templa::traits
{

    /**
     * @brief Recursively strips const, volatile qualifiers and pointers from a type.
     *
     * @tparam T The type to strip.
     *
     * @note This struct removes all levels of const, volatile, and pointer indirection.
     *       For example, strip<const volatile int *>::type is int.
     */
    template <typename T>
    struct strip
    {
        using type = T;
    };

    template <typename T>
    struct strip<const T>
    {
        using type = strip<T>::type;
    };

    template <typename T>
    struct strip<volatile T>
    {
        using type = strip<T>::type;
    };

    template <typename T>
    struct strip<const volatile T>
    {
        using type = strip<T>::type;
    };

    template <typename T>
    struct strip<T *>
    {
        using type = strip<T>::type;
    };

    template <typename T>
    struct strip<const T *>
    {
        using type = strip<T>::type;
    };

    template <typename T>
    struct strip<volatile T *>
    {
        using type = strip<T>::type;
    };

    template <typename T>
    struct strip<const volatile T *>
    {
        using type = strip<T>::type;
    };

    /**
     * @brief Helper alias for strip.
     *
     * Usage:
     * @code
     * using base_t = templa::traits::strip_t<const volatile int*>;
     * // base_t is int
     * @endcode
     */
    template <typename T>
    using strip_t = strip<T>::type;

    /**
     * @brief Extracts the return type of a callable or function signature.
     *
     * @tparam T Callable type or function signature.
     *
     * This template provides the nested member `type` representing the return type.
     *
     * Specializations:
     * - For function signatures: `return_type<R(Args...)>` extracts R.
     * - For std::function: extracts the return type R.
     *
     * @note Uses `std::invoke_result_t` for general callables.
     */
    template <typename R, typename... Args>
    struct return_type
    {
        using type = std::invoke_result_t<R, Args...>;
    };

    template <typename R, typename... Args>
    struct return_type<R(Args...)>
    {
        using type = R;
    };

    template <typename R, typename... Args>
    struct return_type<std::function<R(Args...)>>
    {
        using type = R;
    };

    /**
     * @brief Helper alias to extract return type.
     */
    template <typename T>
    using return_type_t = return_type<T>::type;

    /**
     * @brief Trait to check if two callable types have the same return type when called with Args...
     *
     * @tparam T First callable type.
     * @tparam U Second callable type.
     * @tparam Args Argument types to call with.
     *
     * Provides constexpr static bool member `value`.
     */
    template <typename T, typename U, typename... Args>
    struct is_same_return_type_callable
    {
        constexpr static bool value = std::is_same_v<std::invoke_result_t<T, Args...>, std::invoke_result_t<U, Args...>>;
    };

    /**
     * @brief Convenience variable template for checking if two types have the same return type.
     */
    template <typename T, typename U>
    constexpr static bool is_same_return_type_v = std::is_same_v<return_type_t<T>, return_type_t<U>>;

    /**
     * @brief Convenience variable template for checking if two callables have the same return type given Args.
     */
    template <typename T, typename U, typename... Args>
    constexpr static bool is_same_return_type_callable_v = is_same_return_type_callable<T, U, Args...>::value;
}
