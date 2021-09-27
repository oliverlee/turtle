#pragma once

#include "fwd.hpp"
#include "meta.hpp"
#include "orientation.hpp"

#include <tuple>

namespace turtle {

template <class FrameTree, class... Orientations>
requires std::conjunction_v<
    meta::is_specialization_of<FrameTree, meta::type_tree>,
    std::is_same<typename FrameTree::parent_type,
                 typename std::tuple_element_t<0, std::tuple<Orientations...>>::from_type>,
    std::conjunction<std::is_same<typename FrameTree::parent_type::scalar_type,
                                  typename Orientations::scalar_type>...>>
struct world : Orientations... {
    using frame_tree_type = FrameTree;

    template <class From, class To>
    constexpr auto orientation() & noexcept
        -> decltype(static_cast<turtle::orientation<From, To>>(*this))
    {
        return static_cast<turtle::orientation<From, To>>(*this);
    }
    template <class From, class To>
    constexpr auto orientation() const& noexcept
        -> decltype(static_cast<turtle::orientation<From, To>>(*this))
    {
        return static_cast<turtle::orientation<From, To>>(*this);
    }
};

namespace detail {

template <class, class...>
struct make_tree_impl;

template <class Root, class First, class... Next>
struct make_tree_impl<orientation<Root, First>, Next...>
    : make_tree_impl<meta::type_tree<Root, First>, Next...> {};

template <class... Nodes, class A, class B, class... Next>
struct make_tree_impl<meta::type_tree<Nodes...>, orientation<A, B>, Next...>
    : make_tree_impl<typename meta::type_tree<Nodes...>::template add_branch_t<A, B>, Next...> {};

template <class... Nodes>
struct make_tree_impl<meta::type_tree<Nodes...>> {
    using type = meta::type_tree<Nodes...>;
};

template <class... Ts>
struct make_tree : make_tree_impl<Ts...> {};

template <class... Ts>
using make_tree_t = typename make_tree<Ts...>::type;

}  // namespace detail

template <class... Orientations>
world(Orientations...) -> world<detail::make_tree_t<Orientations...>, Orientations...>;

}  // namespace turtle

// TODO world formatter
