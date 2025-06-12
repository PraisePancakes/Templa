#pragma once

namespace templa
{
    /**
     * @brief Compile-time unrolled loop that applies a callable with a compile-time index sequence.
     *
     * This function template performs a static (compile-time) iteration from 0 to N-1,
     * invoking the provided callable `f` with the expanded indices as template arguments.
     * It forwards any additional arguments to the callable.
     *
     * @tparam N Number of iterations (compile-time constant).
     * @tparam F Type of the callable object.
     * @tparam Args Types of additional arguments forwarded to the callable.
     * @param f Callable to invoke with template indices.
     * @param args Additional arguments to forward to the callable.
     * @return The result of the callable invocation (if any).
     *
     * @note The callable `f` must have a templated call operator accepting template parameter pack of std::size_t.
     * 
     */
    template <std::size_t N, typename F, typename... Args>
    [[nodiscard]] constexpr auto static_for(F &&f, Args &&...args)
    {
        return []<std::size_t... I>(std::index_sequence<I...>, F &&f, Args &&...args) constexpr
        {
            std::forward<F>(f).template operator()<I...>(std::forward<Args>(args)...);
        }(std::make_index_sequence<N>{}, std::forward<F>(f), std::forward<Args>(args)...);
    };
};