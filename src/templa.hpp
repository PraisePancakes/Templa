#pragma once
#include <iostream>
#include <tuple>
#include <variant>
#include <array>
#include <cstdint>
#include <vector>
#include <functional>

namespace templa
{

    namespace internal
    {
        namespace hidden
        {
            template <typename... Ts>
            struct pack;
        }

        template <typename... Ts>
        struct type_list
        {
            using type = hidden::pack<Ts...>;
            constexpr static size_t size = sizeof...(Ts);
        };

        template <template <typename...> class T, typename... Ts>
        struct type_list<T<Ts...>>
        {
            using type = type_list<Ts...>;
        };

    }

    namespace convert
    {
        template <typename... Ts>
        struct convert_to_variant : std::type_identity<std::variant<Ts...>>
        {
        };

        template <template <typename...> class From, typename... Ts>
        struct convert_to_variant<From<Ts...>> : std::type_identity<std::variant<Ts...>>
        {
        };

        template <typename... Ts>
        struct convert_to_tuple : std::type_identity<std::tuple<Ts...>>
        {
        };

        template <template <typename...> class From, typename... Ts>
        struct convert_to_tuple<From<Ts...>> : std::type_identity<std::tuple<Ts...>>
        {
        };
    }

    template <typename Initial, typename... Ts>
    struct type_list_append : internal::type_list<Initial, Ts...>
    {
    };

    template <template <typename...> class Initial, template <typename...> class Appender, typename... Inits, typename... Apps>
    struct type_list_append<Initial<Inits...>, Appender<Apps...>> : internal::type_list<Inits..., Apps...>
    {
    };

    template <template <typename...> class Initial, typename Elem, typename... Inits>
    struct type_list_append<Initial<Inits...>, Elem> : internal::type_list<Inits..., Elem>
    {
    };

    template <typename Initial, typename... Preps>
    struct type_list_prepend : internal::type_list<Preps..., Initial>
    {
    };

    template <template <typename...> class Initial, template <typename...> class Prepender, typename... Inits, typename... Preps>
    struct type_list_prepend<Initial<Inits...>, Prepender<Preps...>> : internal::type_list<Preps..., Inits...>
    {
    };

    template <template <typename...> class Initial, typename Elem, typename... Inits>
    struct type_list_prepend<Initial<Inits...>, Elem> : internal::type_list<Elem, Inits...>
    {
    };

    // helper declaration
    template <typename... T>
    struct type_list_pop_front;

    template <typename T, typename... Ts>
    struct type_list_pop_front<T, Ts...> : internal::type_list<Ts...>
    {
        using popped = T;
    };

    template <template <typename...> class Tlist, typename U, typename... Ts>
    struct type_list_pop_front<Tlist<U, Ts...>> : internal::type_list<Ts...>
    {
        using popped = U;
    };

    template <typename... Ts>
    struct type_list_pop_back;

    template <typename Head, typename Tail>
    struct type_list_pop_back<Head, Tail> : internal::type_list<Head>
    {
        using popped = Tail;
    };

    template <typename Head, typename Mid, typename... Tail>
    struct type_list_pop_back<Head, Mid, Tail...>
    {

    private:
        using type_list = type_list_prepend<typename type_list_pop_back<Mid, Tail...>::type, Head>;

    public:
        using type = type_list::type;
        constexpr static size_t size = type::size;
        using popped = typename type_list_pop_back<Mid, Tail...>::popped;
    };

    template <template <typename...> class Tlist, typename... Ts>
    struct type_list_pop_back<Tlist<Ts...>>
    {
    private:
        using type_list = type_list_pop_back<Ts...>;

    public:
        using type = type_list::type;
        constexpr static size_t size = type::size;
        using popped = type_list::popped;
    };

    namespace internal
    {

        template <typename... Fns>
        struct visitor : Fns...
        {
            using Fns::operator()...;
        };
    };

    template <std::size_t x>
    using value = std::integral_constant<std::size_t, x>;

    template <std::size_t idx, typename... Ts>
    struct type_at_index
    {
        static_assert(idx <= sizeof...(Ts) && idx >= 0);

        using type = decltype([]<std::size_t... i>(std::index_sequence<i...>)
                              { return internal::visitor{
                                    [](value<i>)
                                    {
                                        return std::type_identity<Ts>();
                                    }...}(value<idx>{}); }(std::index_sequence_for<Ts...>()))::type;
    };

    template <std::size_t idx, template <typename...> class T, typename... Ts>
    struct type_at_index<idx, T<Ts...>>
    {
        using type = type_at_index<idx, Ts...>::type;
    };

    template <std::size_t idx, typename... Ts>
    using type_at_index_t = type_at_index<idx, Ts...>::type;

    template <typename T, typename... List>
    struct index_at_type
    {
        constexpr static std::size_t index = []()
        {
            return []<std::size_t... i>(std::index_sequence<i...>)
            {
                return internal::visitor{
                    [](std::type_identity<List>)
                    {
                        return value<i>();
                    }...}(std::type_identity<T>{});
            }(std::index_sequence_for<List...>{});
        }();
    };

    template <typename T, template <typename...> class U, typename... List>
    struct index_at_type<T, U<List...>>
    {
        constexpr static std::size_t index = index_at_type<T, List...>::index;
    };

    template <typename T, typename... List>
    constexpr static std::size_t index_at_type_v = index_at_type<T, List...>::index;

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

    namespace type_info
    {

        template <typename T>
        struct name_of
        {
            static constexpr auto value = "";
        };

        template <>
        struct name_of<int>
        {
            static constexpr auto value = "int";
        };

        template <>
        struct name_of<char>
        {
            static constexpr auto value = "char";
        };

        template <>
        struct name_of<float>
        {
            static constexpr auto value = "float";
        };

        template <>
        struct name_of<long>
        {
            static constexpr auto value = "long";
        };

        template <>
        struct name_of<void>
        {
            static constexpr auto value = "void";
        };

        template <>
        struct name_of<std::uint8_t>
        {
            static constexpr auto value = "uint8_t";
        };

        template <>
        struct name_of<std::uint16_t>
        {
            static constexpr auto value = "uint16_t";
        };

        template <>
        struct name_of<std::uint64_t>
        {
            static constexpr auto value = "uint64_t";
        };

        template <>
        struct name_of<std::uint32_t>
        {
            static constexpr auto value = "uint32_t";
        };

        template <typename T>
        struct type_descriptor
        {

            constexpr static std::string stringify()
            {
                if constexpr (std::is_const_v<T>)
                {
                    return "const " + type_descriptor<std::remove_const_t<T>>().stringify();
                }
                if constexpr (std::is_pointer_v<T>)
                {
                    return type_descriptor<std::remove_pointer_t<T>>().stringify() + "*";
                }
                if constexpr (std::is_reference_v<T>)
                {
                    return type_descriptor<std::remove_reference_t<T>>().stringify() + "&";
                }

                return name_of<T>::value;
            };
        };

    }
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

    }

    namespace algorithms
    {

        template <typename T, std::size_t N, std::size_t M>
        constexpr static auto concat(const std::array<T, N> &lhs, const std::array<T, M> &rhs)
        {
            std::array<T, N + M> new_arr;
            size_t new_index = 0;
            for (size_t i = 0; i < N; i++)
            {
                new_arr[new_index] = lhs[i];
                new_index++;
            }

            for (size_t j = 0; j < M; j++)
            {
                new_arr[new_index] = rhs[j];
                new_index++;
            }

            return new_arr;
        };

        template <const std::string_view &...Strs>
        struct join
        {
        private:
            constexpr static auto impl()
            {
                constexpr std::size_t length = (Strs.size() + ... + 0);
                std::array<char, length + 1> arr;
                auto Joiner = [i = 0, &arr](const std::string_view &s) mutable
                {
                    for (auto &c : s)
                    {
                        arr[i++] = c;
                    }
                };
                (Joiner(Strs), ...);
                arr[length] = 0;
                return arr;
            };

        public:
            constexpr static auto arr = impl();
            constexpr static std::string_view value = {arr.data(), arr.size() - 1};
        };

        template <std::string_view const &...Strs>
        constexpr static std::string_view join_v = join<Strs...>::value;

        template <typename T>
            requires std::input_or_output_iterator<T>
        void reverse(T begin, T end)
        {
            if (begin == end)
                return;
            end--;
            while (begin < end)
            {
                std::swap(*begin, *end);
                begin++;
                end--;
            }
        };

        template <typename T, typename O>
        void flatten(const std::vector<T> &in, std::vector<O> &out)
        {
            out.insert(out.end(), in.begin(), in.end());
        };

        template <typename T, typename O>
        void flatten(const std::vector<std::vector<T>> &in, std::vector<O> &out)
        {
            for (const auto &e : in)
            {
                flatten(e, out);
            };
        };

    };

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

    namespace ctti
    {
        template <typename C>
        struct hash_t
        {
        private:
            constexpr static int _var{0};

        public:
            constexpr static auto id{&_var};
        };
    }
};