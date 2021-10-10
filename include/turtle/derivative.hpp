#pragma once

#include "fwd.hpp"

#include "fmt/format.h"

#include <cstddef>

namespace turtle {

/// @brief A derivative of a vector quantity, taken with respect to a reference
/// frame
/// @tparam B Observation frame
/// @tparam V Kinematic vector, where V::frame is the expression frame
/// @tparam D Derivative degree
template <kinematic::frame B, kinematic::vector V, std::size_t D>
struct derivative {
    using observation_frame = B;
    using expression_frame = typename V::frame;
    static constexpr auto degree = D;

    V value;
};

template <kinematic::frame B, kinematic::vector V>
constexpr auto velocity(V v) -> derivative<B, V, 1>
{
    return {std::move(v)};
}
template <kinematic::vector V>
constexpr auto velocity(V v) -> derivative<typename V::frame, V, 1>
{
    return {std::move(v)};
}

}  // namespace turtle

namespace fmt {

template <class B, class V, std::size_t D>
struct formatter<turtle::derivative<B, V, D>> : formatter<V> {
    template <class FormatContext>
    auto format(const turtle::derivative<B, V, D>& d, FormatContext& ctx)
    {
        auto&& out = ctx.out();

        format_to(out, "d/dt <{}, {}> ", B{}, D);
        formatter<V>::format(d.value, ctx);

        return out;
    }
};

}  // namespace fmt
