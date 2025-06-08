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
        concept Comparable = requires(T a, T b) {
            { a < b };
            { a == b };
            { a > b };
            { a != b };
            { a >= b };
            { a <= b };
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

        template <class E>
        concept DefaultErasable = requires(E *p) {
            std::destroy_at(p);
        };

        template <class E, class T, class A>
        concept AllocatorErasable = requires(A m, E *p) {
            requires std::same_as<typename T::allocator_type, typename std::allocator_traits<A>::rebind_alloc<E>>;
            std::allocator_traits<A>::destroy(m, p);
        };

        template <class T>
        concept AllocatorAware = requires(T a) {
            { a.get_allocator() } -> std::same_as<typename T::allocator_type>;
        };

        template <class T>
        struct is_basic_string : std::false_type
        {
        };

        template <class C, class T, class A>
        struct is_basic_string<std::basic_string<C, T, A>> : std::true_type
        {
        };

        template <class T>
        constexpr bool is_basic_string_v = is_basic_string<T>::value;

        template <class E, class T>
        concept Erasable = (is_basic_string_v<T> && DefaultErasable<E>) || (AllocatorAware<T> && AllocatorErasable<E, T, typename T::allocator_type>) || (!AllocatorAware<T> && DefaultErasable<E>);

        template <class T>
        concept Container = requires(T a, const T b) {
            requires std::regular<T>;
            requires std::swappable<T>;
            requires Erasable<typename T::value_type, T>;
            requires std::same_as<typename T::reference, typename T::value_type &>;
            requires std::same_as<typename T::const_reference, const typename T::value_type &>;
            requires std::forward_iterator<typename T::iterator>;
            requires std::forward_iterator<typename T::const_iterator>;
            requires std::signed_integral<typename T::difference_type>;
            requires std::same_as<typename T::difference_type, typename std::iterator_traits<typename T::iterator>::difference_type>;
            requires std::same_as<typename T::difference_type, typename std::iterator_traits<typename T::const_iterator>::difference_type>;
            { a.begin() } -> std::same_as<typename T::iterator>;
            { a.end() } -> std::same_as<typename T::iterator>;
            { b.begin() } -> std::same_as<typename T::const_iterator>;
            { b.end() } -> std::same_as<typename T::const_iterator>;
            { a.cbegin() } -> std::same_as<typename T::const_iterator>;
            { a.cend() } -> std::same_as<typename T::const_iterator>;
            { a.size() } -> std::same_as<typename T::size_type>;
            { a.max_size() } -> std::same_as<typename T::size_type>;
            { a.empty() } -> std::convertible_to<bool>;
        };
    }
}