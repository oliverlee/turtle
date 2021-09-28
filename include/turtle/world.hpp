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
    using root = typename FrameTree::parent_type;

    template <class From, class To>
    constexpr auto get() & noexcept -> decltype(static_cast<orientation<From, To>>(*this))
    {
        return static_cast<orientation<From, To>>(*this);
    }
    template <class From, class To>
    constexpr auto get() const& noexcept -> decltype(static_cast<orientation<From, To>>(*this))
    {
        return static_cast<orientation<From, To>>(*this);
    }

  private:
    template <class O, class End>
    constexpr auto compose_path(O ori, meta::type_list<End>) const -> O
    {
        return ori;
    }
    template <class O, class A, class B, class... Frames>
    constexpr auto compose_path(O ori, meta::type_list<A, B, Frames...>) const
        -> decltype(compose_path(std::move(ori) * get<A, B>(), meta::type_list<B, Frames...>{}))
    {
        return compose_path(std::move(ori) * get<A, B>(), meta::type_list<B, Frames...>{});
    }

  public:
    template <class To>
    constexpr auto express() const
        -> decltype(compose_path(orientation<root, root>{}, meta::path_t<To, frame_tree_type>{}))
    {
        return compose_path(orientation<root, root>{}, meta::path_t<To, frame_tree_type>{});
    }

    template <class From, class To>
    constexpr auto express() const -> orientation<From, To>
    {

        return express<From>().inverse() * express<To>();
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
