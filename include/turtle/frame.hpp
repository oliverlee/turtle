#pragma once

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

template <detail::Descriptor Name>
struct frame {
    static constexpr auto name = std::string_view{Name.name.data()};
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
