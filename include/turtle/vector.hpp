#pragma once

#include "fmt/format.h"

#include <array>
#include <iterator>
#include <type_traits>

namespace turtle {

template <class T>
class vector {
    using data_type = std::array<T, 3>;

    data_type data_{};

  public:
    using value_type = T;

    using iterator = typename data_type::iterator;
    using const_iterator = typename data_type::const_iterator;

    constexpr vector() = default;

    template <class U, class = std::enable_if_t<std::is_same_v<U, T>>>
    constexpr vector(U x, U y, U z) : data_{std::move(x), std::move(y), std::move(z)}
    {}

    constexpr auto x() & -> T& { return std::get<0>(data_); }
    constexpr auto x() && -> T&& { return std::get<0>(data_); }
    constexpr auto x() const& -> const T& { return std::get<0>(data_); }

    constexpr auto y() & -> T& { return std::get<1>(data_); }
    constexpr auto y() && -> T&& { return std::get<1>(data_); }
    constexpr auto y() const& -> const T& { return std::get<1>(data_); }

    constexpr auto z() & -> T& { return std::get<2>(data_); }
    constexpr auto z() && -> T&& { return std::get<2>(data_); }
    constexpr auto z() const& -> const T& { return std::get<2>(data_); }

    constexpr auto begin() & -> iterator { return data_.begin(); }
    constexpr auto begin() const& -> const_iterator { return data_.begin(); }
    constexpr auto cbegin() const& -> const_iterator { return data_.cbegin(); }

    constexpr auto end() & -> iterator { return data_.end(); }
    constexpr auto end() const& -> const_iterator { return data_.end(); }
    constexpr auto cend() const& -> const_iterator { return data_.cend(); }
};

template <class T>
vector(T x, T y, T z) -> vector<T>;

}  // namespace turtle

template <class T>
struct fmt::formatter<turtle::vector<T>> : fmt::formatter<T> {
    template <class FormatContext>
    auto format(const turtle::vector<T>& v, FormatContext& ctx)
    {
        auto&& out = ctx.out();

        format_to(out, "(");
        formatter<T>::format(v.x(), ctx);
        format_to(out, ", ");
        formatter<T>::format(v.y(), ctx);
        format_to(out, ", ");
        formatter<T>::format(v.z(), ctx);
        format_to(out, ")");

        return out;
    }
};
