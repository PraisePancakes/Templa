#pragma once

namespace templa
{
    template <typename E>
    struct FlagEnum
    {
        static_assert(std::is_enum<E>::value, "E must be enum type");
        using _UTY = typename std::underlying_type<E>::type;
        _UTY mask = 0x000;

        constexpr FlagEnum<E>  &operator|=(E f)
        {
            mask |= static_cast<_UTY>(f);
            return *this;
        };

        constexpr FlagEnum<E> &operator&=(E f)
        {
            mask &= static_cast<_UTY>(f);
            return *this;
        }

        constexpr bool operator&(E f) const
        {
            return (mask & static_cast<_UTY>(f)) == static_cast<_UTY>(f);
        }
        constexpr FlagEnum<E>  & operator|(E f) 
        {
             mask |= static_cast<_UTY>(f);
            return *this;
        }

        template <typename U>
        constexpr bool operator==(const FlagEnum<U> &o) const
        {
            if constexpr (std::is_same<_UTY, typename FlagEnum<U>::_UTY>::value &&
                          std::is_same<E, U>::value)
            {
                return true;
            }
            return false;
        }

        template <typename U>
        constexpr bool operator!=(const FlagEnum<U> &o) const
        {
            return !(*this == o);
        }

        constexpr void SetFlag(E f) { *this |= f; };
        constexpr bool HasFlag(E f) { return *this & f; };
    };
};