#pragma once

#include "meta.hpp"

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <type_traits>

namespace turtle {

/// @brief Concepts for kinematic types
namespace kinematic {}  // namespace kinematic

/// @brief Default scalar type used in turtle types.
using DefaultScalar = double;

/// @brief Implementation detail types
namespace detail {

/// @brief Helper for specifying a frame's name as a string literal
/// @tparam N Size of name string literal
template <std::size_t N>
struct descriptor {
    constexpr descriptor(const char (&str)[N])
    {
        std::copy_n(str, N, name.data());
    }

    std::array<char, N> name{};
};

}  // namespace detail

template <detail::descriptor Name, class T>
struct frame;

namespace detail {

/// @brief Checks whether T is a frame type
template <class T, class = void>
struct is_frame : std::false_type {};

/// @brief Specialization if T is a specialization of frame
template <detail::descriptor Name, class T>
struct is_frame<frame<Name, T>> : std::true_type {};

}  // namespace detail

/// @name Type traits
/// @{

/// @brief Checks whether T is a reference frame
template <class T>
using is_frame = detail::is_frame<T>;

/// @}

/// @name Helper variable templates
/// @{

template <class T>
inline constexpr bool is_frame_v = is_frame<T>::value;

/// @}

namespace kinematic {

/// @brief Specifies that a type is a reference frame
template <class T>
concept frame = is_frame_v<T>;

}  // namespace kinematic

template <kinematic::frame Frame>
class vector;

/// @name Type traits
/// @{

/// @brief Checks whether T is a kinematic vector
template <class T>
using is_vector = meta::is_specialization_of<T, vector>;

/// @}

/// @name Helper variable templates
/// @{

template <class T>
inline constexpr bool is_vector_v = is_vector<T>::value;

/// @}

namespace kinematic {

/// @brief Specifies that a type is a kinematic vector
template <class T>
concept vector = is_vector_v<T>;

}  // namespace kinematic

template <class T>
class quaternion;

template <kinematic::frame From, kinematic::frame To>
requires std::same_as<typename From::scalar, typename To::scalar>
class orientation;

/// @name Type traits
/// @{

/// @brief Checks whether T is an orientation
template <class T>
using is_orientation = meta::is_specialization_of<T, orientation>;

/// @}

/// @name Helper variable templates
/// @{

template <class T>
inline constexpr bool is_orientation_v = is_orientation<T>::value;

/// @}

namespace kinematic {

/// @brief Specifies that a type is an orientation
template <class T>
concept orientation = is_orientation_v<T>;

}  // namespace kinematic

template <class FrameTree, kinematic::orientation... Os>
requires std::conjunction_v<
    meta::is_specialization_of<FrameTree, meta::tree>,
    std::is_same<typename FrameTree::root,
                 typename meta::first_t<Os...>::source_frame>,
    std::conjunction<
        std::is_same<typename FrameTree::root::scalar, typename Os::scalar>...>>
class world;

/// @name Type traits
/// @{

/// @brief Checks whether T is a world
template <class T>
using is_world = meta::is_specialization_of<T, world>;

/// @}

/// @name Helper variable templates
/// @{

template <class T>
inline constexpr bool is_world_v = is_world<T>::value;

/// @}

namespace kinematic {

/// @brief Specifies that a type is a kinematic world
template <class T>
concept world = is_world_v<T>;

}  // namespace kinematic

template <kinematic::world World>
class point;

}  // namespace turtle
