#pragma once

namespace templa
{
    template <std::size_t N, typename F, typename... Args>
    [[nodiscard]] constexpr auto static_for(F &&f, Args &&...args)
    {
        return []<std::size_t... I>(std::index_sequence<I...>, F &&f, Args &&...args) constexpr
        {
            std::forward<F>(f).template operator()<I...>(std::forward<Args>(args)...);
        }(std::make_index_sequence<N>{}, std::forward<F>(f), std::forward<Args>(args)...);
    };
};