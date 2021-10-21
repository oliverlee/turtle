#pragma once

#include "fwd.hpp"
#include "util/zip_transform_iterator.hpp"
#include "vector_interface.hpp"

#include "fmt/format.h"
#include "fmt/ranges.h"

#include <type_traits>

namespace turtle {

/// @brief Reference frame bound 3D vector
/// @tparam F Kinematic reference frame
///
/// A 3D vector belonging to a reference frame. A vector value is always tied to
/// the associated frame F but may be expressed in another frame via a world
/// instance.
template <kinematic::frame F, class T = typename F::scalar>
struct vector : vector_interface<T, vector<F>> {
    // tparam `T` allows use when defining frame basis vectors with an
    // incomplete frame type
    static_assert(std::is_same_v<typename F::scalar, T>);

    /// @name Kinematic types
    /// @{

    /// @brief Vector associated frame
    using frame = F;

    /// @}

    using vector_interface<T, vector<F>>::vector_interface;
};

}  // namespace turtle

namespace fmt {

template <class F, class Char>
struct is_range<turtle::vector<F>, Char> : std::false_type {};

template <class F>
struct formatter<turtle::vector<F>> : formatter<typename F::scalar> {
    template <class FormatContext>
    auto format(const turtle::vector<F>& v, FormatContext& ctx)
    {
        using T = typename F::scalar;

        auto&& out = ctx.out();

        format_to(out, "[{}] (", F::name);
        formatter<T>::format(v.x(), ctx);
        format_to(out, ", ");
        formatter<T>::format(v.y(), ctx);
        format_to(out, ", ");
        formatter<T>::format(v.z(), ctx);
        format_to(out, ")");

        return out;
    }
};

}  // namespace fmt
