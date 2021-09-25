#pragma once

#include "vector.hpp"

#include <algorithm>
#include <array>
#include <fmt/format.h>
#include <string_view>

namespace turtle {

namespace detail {

template <std::size_t N>
struct Descriptor {
    constexpr Descriptor(const char (&str)[N]) { std::copy_n(str, N, name.data()); }

    std::array<char, N> name{};
};

}  // namespace detail

template <detail::Descriptor Name, class T = double>
struct frame {
    using scalar_type = T;
    using vector = turtle::vector<frame>;

    static constexpr auto name = std::string_view{Name.name.data()};

    static constexpr vector x{T{1}, T{}, T{}};
    static constexpr vector y{T{}, T{1}, T{}};
    static constexpr vector z{T{}, T{}, T{1}};
};

}  // namespace turtle

template <turtle::detail::Descriptor Name>
struct fmt::formatter<turtle::frame<Name>> : fmt::formatter<std::string_view> {
    template <class FormatContext>
    auto format(const turtle::frame<Name>&, FormatContext& ctx)
    {
        return fmt::formatter<std::string_view>::format(Name.name.data(), ctx);
    }
};
