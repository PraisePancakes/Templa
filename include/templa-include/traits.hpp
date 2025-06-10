#pragma once
#include <iostream>
#include <functional>

namespace templa
{
    namespace traits
    {
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

        template <typename T>
        using strip_t = strip<T>::type;

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

        template <typename T>
        using return_type_t = return_type<T>::type;

        template <typename T, typename U, typename... Args>
        struct is_same_return_type_callable;

        template <typename T, typename U, typename... Args>
        struct is_same_return_type_callable
        {
            constexpr static bool value = std::is_same_v<std::invoke_result_t<T, Args...>, std::invoke_result_t<U, Args...>>;
        };

        template <typename T, typename U>
        constexpr static bool is_same_return_type_v = std::is_same_v<return_type_t<T>, return_type_t<U>>;

        template <typename T, typename U, typename... Args>
        constexpr static bool is_same_return_type_callable_v = is_same_return_type_callable<T, U, Args...>::value;
    }
};