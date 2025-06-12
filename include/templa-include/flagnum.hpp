#pragma once
/** \defgroup flag_enum
 *
 */

/** \file
 *  \brief A utility wrapper for using scoped or unscoped enum types as bit flags.
 *  \ingroup flag_enum
 */
namespace templa
{
    /**
     * @brief A utility wrapper for using scoped or unscoped enum types as bit flags.
     *
     * Provides a type-safe way to combine, test, and manipulate enum flags using bitwise operations.
     *
     * @tparam ENUM The enumeration type. Must satisfy `std::is_enum`.
     *
     * @note The enum values are implicitly cast to their underlying integral type.
     *
     */
    template <typename ENUM>
    struct FlagEnum
    {
        static_assert(std::is_enum<ENUM>::value, "ENUM must be an enum type");

        /// The underlying integral type used for bitwise operations.
        using underlying_t = typename std::underlying_type<ENUM>::type;

        /// Bitmask storing the combined flag state.
        underlying_t mask = 0x000;

        /// Default constructor. Initializes with no flags set.
        FlagEnum() : mask{0} {}

        /// Constructs from a single enum flag.
        FlagEnum(ENUM single_flag) : mask(static_cast<underlying_t>(single_flag)) {}

        /// Copy constructor.
        FlagEnum(const FlagEnum &o) : mask(o.mask) {}

        /// Default destructor.
        ~FlagEnum() = default;

        /**
         * @brief Bitwise OR assignment with an enum value.
         * @param addValue Enum value to OR into the mask.
         * @return Reference to this object.
         */
        constexpr FlagEnum &operator|=(ENUM addValue)
        {
            mask |= static_cast<underlying_t>(addValue);
            return *this;
        }

        /**
         * @brief Bitwise AND assignment with an enum value.
         * @param maskValue Enum value to AND with.
         * @return Reference to this object.
         */
        constexpr FlagEnum &operator&=(ENUM maskValue)
        {
            mask &= static_cast<underlying_t>(maskValue);
            return *this;
        }

        /**
         * @brief Equality comparison with an enum value.
         * @param maskValue Enum value to test against.
         * @return True if all bits in `maskValue` are set in the current mask.
         */
        constexpr bool operator==(ENUM maskValue) const noexcept
        {
            return ((this->mask & static_cast<underlying_t>(maskValue)) == static_cast<underlying_t>(maskValue));
        }

        /**
         * @brief Boolean conversion operator.
         * @return True if any flag is set; false otherwise.
         */
        explicit operator bool() const noexcept
        {
            return mask != 0;
        }
    };

    /**
     * @brief Enables bitwise OR for enum flags.
     */
    template <typename E>
        requires std::is_enum_v<E>
    constexpr FlagEnum<E> operator|(E lhs, E rhs)
    {
        return FlagEnum<E>(lhs) | rhs;
    }

    /**
     * @brief Enables bitwise AND for enum flags.
     */
    template <typename E>
        requires std::is_enum_v<E>
    constexpr FlagEnum<E> operator&(E lhs, E rhs)
    {
        return FlagEnum<E>(lhs) & rhs;
    }

    /**
     * @brief Enables bitwise NOT for enum flags.
     */
    template <typename E>
        requires std::is_enum_v<E>
    constexpr FlagEnum<E> operator~(E val)
    {
        return ~FlagEnum<E>(val);
    }
};