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
     * @file
     * @brief Traits to extract function signature information.
     * @ingroup type_traits
     */

    /// @defgroup function_traits Function Traits
    /// Compile-time utilities to introspect function signatures.
    /// @{

    /**
     * @brief Primary template for function traits.
     * Specialization used to extract traits from functor types (e.g., lambdas).
     *
     * @tparam F Functor type (i.e., any type with operator()).
     */
    template <class F>
    struct function_traits
    {
    private:
        using call_type = function_traits<decltype(&F::type::operator())>;

    public:
        /// The return type of the function.
        using return_type = typename call_type::return_type;

        /// The number of arguments (excluding the implicit object parameter).
        static constexpr std::size_t arity = call_type::arity - 1;

        /**
         * @brief Retrieves the type of the N-th argument.
         * @tparam N Index of the argument (0-based).
         */
        template <std::size_t N>
        struct argument
        {
            static_assert(N < arity, "error: invalid parameter index.");
            /// The type of the N-th argument.
            using type = typename call_type::template argument<N + 1>::type;
        };
    };

    /**
     * @brief Specialization for regular function types.
     *
     * @tparam R Return type.
     * @tparam Args Argument types.
     */
    template <typename R, typename... Args>
    struct function_traits<R(Args...)>
    {
        /// Return type of the function.
        using return_type = R;

        /// Number of function parameters.
        static constexpr std::size_t arity = sizeof...(Args);

        /**
         * @brief Retrieves the type of the N-th argument.
         * @tparam N Index of the argument (0-based).
         */
        template <std::size_t N>
        struct argument
        {
            static_assert(N < arity, "error: invalid parameter index.");
            /// Type of the N-th argument.
            using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
        };
    };

    /**
     * @brief Specialization for function pointer types.
     */
    template <class R, class... Args>
    struct function_traits<R (*)(Args...)> : public function_traits<R(Args...)>
    {
    };

    /**
     * @brief Specialization for member function pointers.
     */
    template <class C, class R, class... Args>
    struct function_traits<R (C::*)(Args...)> : public function_traits<R(C &, Args...)>
    {
    };

    /**
     * @brief Specialization for const member function pointers.
     */
    template <class C, class R, class... Args>
    struct function_traits<R (C::*)(Args...) const> : public function_traits<R(C &, Args...)>
    {
    };

    /**
     * @brief Specialization for pointer-to-member types.
     */
    template <class C, class R>
    struct function_traits<R(C::*)> : public function_traits<R(C &)>
    {
    };

    /**
     * @brief Lvalue reference specialization.
     */
    template <class F>
    struct function_traits<F &> : public function_traits<F>
    {
    };

    /**
     * @brief Rvalue reference specialization.
     */
    template <class F>
    struct function_traits<F &&> : public function_traits<F>
    {
    };

    /// @}

}
