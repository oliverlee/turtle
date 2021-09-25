#pragma once

#include <cstddef>
#include <type_traits>

namespace turtle {

namespace detail {
template <std::size_t N>
struct Descriptor;
}

template <detail::Descriptor Name, class T>
struct frame;

template <class, class = void>
struct is_frame : std::false_type {};
template <detail::Descriptor Name, class T>
struct is_frame<frame<Name, T>> : std::true_type {};
template <class T>
inline constexpr bool is_frame_v = is_frame<T>::value;

namespace con {

template <class T>
concept reference_frame = is_frame_v<T>;

}

}  // namespace turtle
