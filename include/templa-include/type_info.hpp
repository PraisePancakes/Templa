#pragma once
#include <iostream>
#include <cstdint>
/**
 * @namespace type_info
 * @brief Provides utilities for obtaining string names and descriptors of types at compile time.
 */

/** \defgroup type_info
 *
 */

/** \file
 *  \brief Provides utilities for obtaining string names and descriptors of types at compile time.
 *  \ingroup type_info
 */
namespace templa::type_info
{

    /**
     * @brief Primary template to get the string name of a type.
     *
     * Specializations provide the string literal name for known types.
     *
     * @tparam T The type whose name is to be obtained.
     */
    template <typename T>
    struct name_of
    {
        /// String literal representing the name of the type T.
        static constexpr auto value = "";
    };

    /// Specialization for `int`.
    template <>
    struct name_of<int>
    {
        static constexpr auto value = "int";
    };

    /// Specialization for `char`.
    template <>
    struct name_of<char>
    {
        static constexpr auto value = "char";
    };

    /// Specialization for `float`.
    template <>
    struct name_of<float>
    {
        static constexpr auto value = "float";
    };

    /// Specialization for `long`.
    template <>
    struct name_of<long>
    {
        static constexpr auto value = "long";
    };

    /// Specialization for `void`.
    template <>
    struct name_of<void>
    {
        static constexpr auto value = "void";
    };

    /// Specialization for `std::uint8_t`.
    template <>
    struct name_of<std::uint8_t>
    {
        static constexpr auto value = "uint8_t";
    };

    /// Specialization for `std::uint16_t`.
    template <>
    struct name_of<std::uint16_t>
    {
        static constexpr auto value = "uint16_t";
    };

    /// Specialization for `std::uint64_t`.
    template <>
    struct name_of<std::uint64_t>
    {
        static constexpr auto value = "uint64_t";
    };

    /// Specialization for `std::uint32_t`.
    template <>
    struct name_of<std::uint32_t>
    {
        static constexpr auto value = "uint32_t";
    };

    /**
     * @brief Provides a human-readable string description of the type T, including qualifiers and pointers.
     *
     * This struct recursively strips const, pointer, and reference qualifiers to build a descriptive string.
     *
     * @tparam T The type to describe.
     */
    template <typename T>
    struct type_descriptor
    {
        /**
         * @brief Returns a string describing the type T.
         *
         * Examples:
         * - `const int*`
         * - `float&`
         *
         * @return A string representing the type.
         */
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

    /**
     * @namespace ctti
     * @brief Compile-time type identification utilities.
     */
    namespace ctti
    {
        /**
         * @brief Generates a unique type identifier for a type C at compile time.
         *
         * This uses the address of a static variable as a unique ID for the type.
         *
         * @tparam C The type to identify.
         */
        template <typename C>
        struct hash_t
        {
        private:
            /// Static variable used to generate unique address per type.
            constexpr static int _var{0};

        public:
            /// Unique identifier pointer for the type C.
            constexpr static auto id{&_var};
        };
    }

}
