#pragma once
#include <iostream>
/** \defgroup concepts
 *  Templa concepts for type restrictions.
 */

/** \file
 *  \brief Concepts
 *  \ingroup concepts
 */
namespace templa::concepts
{

    /**
     * @brief Concept to check if a type is hashable via std::hash.
     *
     * Requires that std::hash<T>{}(x) is valid and convertible to std::size_t.
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept Hashable = requires(T x) {
        { std::hash<T>{}(x) } -> std::convertible_to<std::size_t>;
    };

    /**
     * @brief Concept to check if a type is an integral type.
     *
     * Equivalent to std::is_integral_v<T>.
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept Integral = std::is_integral_v<T>;

    /**
     * @brief Concept to check if a callable can be invoked with given argument types.
     *
     * Requires that F can be called with arguments of types T&&...
     *
     * @tparam F Callable type.
     * @tparam T Argument types.
     */
    template <typename F, typename... T>
    concept CallableWith = requires(F f, T &&...t) {
        { f(std::forward<T>(t)...) };
    };

    /**
     * @brief Concept to check if a type supports common comparison operators.
     *
     * Requires operators <, ==, >, !=, >=, <= to be valid on T.
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept Comparable = requires(T a, T b) {
        { a < b };
        { a == b };
        { a > b };
        { a != b };
        { a >= b };
        { a <= b };
    };

    /**
     * @brief Concept to check if a type is an unsigned integral type.
     *
     * Requires std::is_unsigned_v<T> and std::is_integral_v<T>.
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept UnsignedIntegral = std::is_unsigned_v<T> && std::is_integral_v<T>;

    /**
     * @brief Concept to check if a type is a signed integral type.
     *
     * Requires std::is_signed_v<T> and std::is_integral_v<T>.
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept SignedIntegral = std::is_signed_v<T> && std::is_integral_v<T>;

    /**
     * @brief Concept to check if a type supports addition operations.
     *
     * Requires + and += operators.
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept Addable = requires(T a, T b) {
        { a + b };
        { a += b };
    };

    /**
     * @brief Concept to check if a type supports subtraction operations.
     *
     * Requires - and -= operators.
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept Subtractable = requires(T a, T b) {
        { a - b };
        { a -= b };
    };

    /**
     * @brief Concept to check if a type supports multiplication operations.
     *
     * Requires * and *= operators.
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept Multipliable = requires(T a, T b) {
        { a * b };
        { a *= b };
    };

    /**
     * @brief Concept to check if a type supports division operations.
     *
     * Requires / and /= operators.
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept Divisible = requires(T a, T b) {
        { a / b };
        { a /= b };
    };

    /**
     * @brief Concept to check if a type supports all basic arithmetic operations.
     *
     * Requires Addable, Subtractable, Multipliable, and Divisible.
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept Arithmetical = Subtractable<T> && Multipliable<T> && Divisible<T> && Addable<T>;

    /**
     * @brief Concept to check if a type can be streamed to an std::ostream.
     *
     * Requires the expression (os << t) to be valid.
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept Streamable = requires(std::ostream &os, T t) {
        { os << t };
    };

    /**
     * @brief Concept checking if an object can be destroyed via std::destroy_at.
     *
     * @tparam E Type to destroy.
     */
    template <class E>
    concept DefaultErasable = requires(E *p) {
        std::destroy_at(p);
    };

    /**
     * @brief Concept checking if an object can be destroyed via an allocator.
     *
     * Requires allocator A to be rebindable to E and supports destroy(m, p).
     *
     * @tparam E Element type.
     * @tparam T Container type.
     * @tparam A Allocator type.
     */
    template <class E, class T, class A>
    concept AllocatorErasable = requires(A m, E *p) {
        requires std::same_as<typename T::allocator_type, typename std::allocator_traits<A>::rebind_alloc<E>>;
        std::allocator_traits<A>::destroy(m, p);
    };

    /**
     * @brief Concept checking if a type has a get_allocator() method returning allocator_type.
     *
     * @tparam T Type to check.
     */
    template <class T>
    concept AllocatorAware = requires(T a) {
        { a.get_allocator() } -> std::same_as<typename T::allocator_type>;
    };

    /**
     * @brief Helper struct to detect if a type is std::basic_string.
     *
     * Specialized for std::basic_string.
     *
     * @tparam T Type to check.
     */
    template <class T>
    struct is_basic_string : std::false_type
    {
    };

    template <class C, class T, class A>
    struct is_basic_string<std::basic_string<C, T, A>> : std::true_type
    {
    };

    /**
     * @brief Variable template shortcut for is_basic_string.
     */
    template <class T>
    constexpr bool is_basic_string_v = is_basic_string<T>::value;

    /**
     * @brief Concept to check if a container supports erasing of elements.
     *
     * This concept checks if the container either:
     * - is a basic_string and DefaultErasable for its elements, or
     * - is AllocatorAware and supports AllocatorErasable, or
     * - is not AllocatorAware and supports DefaultErasable.
     *
     * @tparam E Element type.
     * @tparam T Container type.
     */
    template <class E, class T>
    concept Erasable = (is_basic_string_v<T> && DefaultErasable<E>) || (AllocatorAware<T> && AllocatorErasable<E, T, typename T::allocator_type>) || (!AllocatorAware<T> && DefaultErasable<E>);

    /**
     * @brief Concept that models an STL-like container.
     *
     * Requires the type to satisfy several container properties including:
     * - Regular, swappable
     * - Supports erasing elements
     * - Provides iterator, const_iterator with forward_iterator concept
     * - Has difference_type and size_type types
     * - Provides begin(), end(), cbegin(), cend(), size(), max_size(), empty()
     *
     * @tparam T Container type to check.
     */
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

    /**
     * \ingroup concepts
     * \brief Checks whether a given type \c T is a specialization of the class template \c Template.
     *
     * This concept is satisfied if \c T is an instantiation of \c Template with any number of template arguments.
     *
     * ### Example
     * \code
     * template <typename...>
     * struct my_template {};
     *
     * static_assert(is_specialization_of<my_template<int>, my_template>);
     * static_assert(!is_specialization_of<int, my_template>);
     * \endcode
     *
     * \tparam T The type to check.
     * \tparam Template The class template to check against (e.g., \c std::tuple, \c std::vector).
     */
    template <class T, template <typename...> class Template>
    concept is_specialization_of = requires(T const &t) {
        []<typename... Args>(Template<Args...> const &)
        { return true; }(t);
    };
}
