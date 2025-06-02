#pragma once
#include <iostream>

namespace templa
{
    namespace concepts
    {
        template <typename T>
        concept Hashable = requires(T x) {
            { std::hash<T>{}(x) } -> std::convertible_to<std::size_t>;
        };

        template <typename T>
        concept Integral = std::is_integral_v<T>;

        template <typename F, typename... T>
        concept CallableWith = requires(F f, T &&...t) {
            { f(std::forward<T>(t)...) };
        };

        template <typename T>
        concept UnsignedIntegral = std::is_unsigned_v<T> && std::is_integral_v<T>;

        template <typename T>
        concept SignedIntegral = std::is_signed_v<T> && std::is_integral_v<T>;

        template <typename T>
        concept Addable = requires(T a, T b) {
            { a + b };
            { a += b };
        };

        template <typename T>
        concept Subtractable = requires(T a, T b) {
            { a - b };
            { a -= b };
        };

        template <typename T>
        concept Multipliable = requires(T a, T b) {
            { a * b };
            { a *= b };
        };

        template <typename T>
        concept Divisible = requires(T a, T b) {
            { a / b };
            { a /= b };
        };

        template <typename T>
        concept Arithmetical = Subtractable<T> && Multipliable<T> && Divisible<T> && Addable<T>;

        template <typename T>
        concept Streamable = requires(std::ostream &os, T t) {
            { os << t };
        };
    }
}