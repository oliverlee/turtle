#pragma once

#include "meta.hpp"

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

template <class T>
using is_vector = meta::is_specialization_of<T, vector>;
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

template <class T>
using is_orientation = meta::is_specialization_of<T, orientation>;
template <class T>
inline constexpr bool is_orientation_v = is_orientation<T>::value;

namespace con {

template <class T>
concept orientation = is_orientation_v<T>;

}

template <class FrameTree, con::orientation... Os>
requires std::conjunction_v<
    meta::is_specialization_of<FrameTree, meta::type_tree>,
    std::is_same<typename FrameTree::parent_type, typename meta::first_t<Os...>::from_type>,
    std::conjunction<
        std::is_same<typename FrameTree::parent_type::scalar_type, typename Os::scalar_type>...>>
class world;


template <class T>
using is_world = meta::is_specialization_of<T, world>;
template <class T>
inline constexpr bool is_world_v = is_world<T>::value;

namespace con {

template <class T>
concept world = is_world_v<T>;

}

}  // namespace turtle
