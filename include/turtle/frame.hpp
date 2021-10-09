#pragma once

#include "fwd.hpp"
#include "vector.hpp"

#include "fmt/format.h"

#include <string_view>

namespace turtle {

/// @brief A reference frame
/// @tparam Name Reference frame description, defined as a string literal
/// @tparam T Scalar type
///
/// A Cartesian reference frame allowing definition of relative distance and
/// motion. These reference frames have no origin and are related to other
/// frames by an `orientation`.
template <detail::descriptor Name, class T = DefaultScalar>
struct frame {
    using scalar = T;  ///< Frame scalar type

    /// @name Kinematic types
    /// @{

    /// @brief Frame vector type
    using vector = turtle::vector<frame>;

    /// @}

    /// @brief Frame descriptor string literal
    static constexpr auto name = std::string_view{Name.name.data()};

    /// @name Frame basis vectors
    /// @{

    /// @brief Unit vector in the frame's x-direction
    static constexpr vector x{T{1}, T{}, T{}};

    /// @brief Unit vector in the frame's y-direction
    static constexpr vector y{T{}, T{1}, T{}};

    /// @brief Unit vector in the frame's z-direction
    static constexpr vector z{T{}, T{}, T{1}};

    /// @}
};

}  // namespace turtle

template <turtle::detail::descriptor Name>
struct fmt::formatter<turtle::frame<Name>> : fmt::formatter<std::string_view> {
    template <class FormatContext>
    auto format(const turtle::frame<Name>&, FormatContext& ctx)
    {
        return fmt::formatter<std::string_view>::format(Name.name.data(), ctx);
    }
};
