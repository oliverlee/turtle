#pragma once

#include <concepts>
#include <cstddef>
#include <type_traits>

namespace turtle {

using DefaultScalar = double;

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

template <con::reference_frame Frame>
class vector;

template <class, class = void>
struct is_vector : std::false_type {};
template <class Frame>
struct is_vector<vector<Frame>, std::enable_if_t<is_frame_v<Frame>>> : std::true_type {};
template <class T>
inline constexpr bool is_vector_v = is_vector<T>::value;

namespace con {

template <class T>
concept reference_frame_vector = is_vector_v<T>;

}

template <class T>
class quaternion;

template <con::reference_frame From, con::reference_frame To>
requires std::same_as<typename From::scalar_type, typename To::scalar_type>
class orientation;

}  // namespace turtle
