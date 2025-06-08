#pragma once

namespace templa
{
    template <typename ENUM>
    struct FlagEnum
    {
        static_assert(std::is_enum<ENUM>::value, "E must be enum type");
        using underlying_t = typename std::underlying_type<ENUM>::type;
        underlying_t mask = 0x000;

        FlagEnum() : mask{0} {};
        FlagEnum(ENUM single_flag) : mask(single_flag) {};
        FlagEnum(const FlagEnum &o) : mask(o.mask) {};
        ~FlagEnum() = default;

        FlagEnum &operator|=(ENUM addValue)
        {
            mask |= static_cast<underlying_t>(addValue);
            return *this;
        }
        FlagEnum operator|(ENUM addValue)
        {
            FlagEnum result(*this);
            result |= addValue;
            return result;
        }
        FlagEnum &operator&=(ENUM maskValue)
        {
            mask &= maskValue;
            return *this;
        }
        FlagEnum operator&(ENUM maskValue)
        {
            FlagEnum result(*this);
            result &= maskValue;
            return result;
        }

        bool operator==(ENUM maskValue)
        {
            return ((this->mask & maskValue) == maskValue);
        };

        FlagEnum operator~()
        {
            FlagEnum result(*this);
            result.mask = ~result.mask;
            return result;
        }
        explicit operator bool() { return mask != 0; }
    };
};