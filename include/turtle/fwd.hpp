#pragma once

#include <cstddef>

namespace turtle {

namespace detail {
template <std::size_t N>
struct Descriptor;
}

template <detail::Descriptor Name>
struct frame;

}  // namespace turtle
