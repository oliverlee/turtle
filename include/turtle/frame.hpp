#pragma once

#include "vector.hpp"

#include "fmt/format.h"

#include <string_view>

namespace turtle {

template <detail::descriptor Name, class T = DefaultScalar>
struct frame {
    using scalar = T;
    using vector = turtle::vector<frame>;

    static constexpr auto name = std::string_view{Name.name.data()};

    static constexpr vector x{T{1}, T{}, T{}};
    static constexpr vector y{T{}, T{1}, T{}};
    static constexpr vector z{T{}, T{}, T{1}};
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
