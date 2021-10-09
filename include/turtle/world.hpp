#pragma once

#include "fwd.hpp"
#include "meta.hpp"
#include "orientation.hpp"

#include "fmt/format.h"
#include "metal.hpp"

#include <cstring>
#include <iterator>
#include <type_traits>
#include <utility>

namespace turtle {

/// @brief A kinematic world
/// @tparam FrameTree Metatype describing a fixed reference frame topology
/// @tparam Os Sequence of frame orientations, corresponding to `FrameTree`
///
/// Specifies reference frame topology and frame relationships at a given
/// instance. This type stores the inter-frame orientations and allows other
/// kinematic types (e.g. vector, point) to be expressed any contained frame,
/// applying underlying rotations.
template <class FrameTree, kinematic::orientation... Os>
requires std::conjunction_v<
    meta::is_specialization_of<FrameTree, meta::tree>,
    std::is_same<typename FrameTree::root,
                 typename meta::first_t<Os...>::source_frame>,
    std::conjunction<
        std::is_same<typename FrameTree::root::scalar, typename Os::scalar>...>>
class world : Os... {
  public:
    /// @name Kinematic types
    /// @{

    /// @brief World frame topology
    using tree = FrameTree;

    /// @brief World inertial frame
    using root = typename FrameTree::root;

    /// @brief World point type
    using point = turtle::point<world>;

    /// @}

    using scalar = typename root::scalar;  ///< World scalar type

    /// @brief Constructs a world with identity rotations relating frames
    constexpr world() = default;

    /// @brief Constructs a world with specified rotations relating frames
    /// @tparam Args Frame orientation types
    /// @param args Frame orientation values
    template <class... Args>
    constexpr world(Args&&... args) : Os(std::forward<Args>(args))...
    {}

    /// @brief Accesses the orientation relationship between two frames
    /// @tparam From Source frame
    /// @tparam To Destination frame
    ///
    /// Accesses the orientation relationship between a source frame and
    /// destination frame, where source is defined to be close the world root.
    template <class From, class To>
    constexpr auto get() & noexcept -> orientation<From, To>&
    {
        return static_cast<orientation<From, To>&>(*this);
    }

    /// @copydoc get
    template <class From, class To>
    [[nodiscard]] constexpr auto get() const& noexcept
        -> const orientation<From, To>&
    {
        return static_cast<const orientation<From, To>&>(*this);
    }

  private:
    template <class O, class End>
    [[nodiscard]] constexpr auto compose_path(O&& ori, metal::list<End>) const
        -> O&&
    {
        return std::forward<O>(ori);
    }
    template <class O, class A, class B, class... Frames>
    [[nodiscard]] constexpr auto
    compose_path(O&& ori, metal::list<A, B, Frames...>) const
    {
        return std::forward<O>(ori) *
               compose_path(get<A, B>(), metal::list<B, Frames...>{});
    }

  public:
    /// @brief Expresses the orientation from the world root to a destination
    /// frame
    /// @tparam To Destination frame
    ///
    /// Composes rotations along the path from world root to frame `To`,
    /// returning the composed orientation of `To` relative root.
    template <kinematic::frame To>
    [[nodiscard]] auto express() const
        -> std::enable_if_t<tree::template contains_v<To>,
                            orientation<root, To>>
    {
        return compose_path(
            orientation<root, root>{}, typename tree::template path_to_t<To>{});
    }

    /// @brief Expresses the orientation of one frame relative another
    /// @tparam From Source frame
    /// @tparam To Destination frame
    ///
    /// Composes rotations along the path from frame `From` to world root to
    /// frame `To`, returning the composed orientation of `To` relative `From`.
    template <kinematic::frame From, kinematic::frame To>
    [[nodiscard]] constexpr auto express() const -> std::enable_if_t<
        tree::template contains_v<From> && tree::template contains_v<To>,
        orientation<From, To>>
    {
        return express<From>().inverse() * express<To>();
    }
};

namespace detail {

template <class, class...>
struct make_tree_impl;

template <class Root, class First, class... Next>
struct make_tree_impl<orientation<Root, First>, Next...>
    : make_tree_impl<meta::tree<Root, First>, Next...> {};

template <class... Nodes, class A, class B, class... Next>
struct make_tree_impl<meta::tree<Nodes...>, orientation<A, B>, Next...>
    : make_tree_impl<typename meta::tree<Nodes...>::template add_branch_t<A, B>,
                     Next...> {};

template <class... Nodes>
struct make_tree_impl<meta::tree<Nodes...>> {
    using type = meta::tree<Nodes...>;
};

template <class... Ts>
struct make_tree : make_tree_impl<Ts...> {};

template <class... Ts>
using make_tree_t = typename make_tree<Ts...>::type;

}  // namespace detail

/// @name Deduction guides
/// @{

template <class... Os>
world(Os...) -> world<detail::make_tree_t<Os...>, Os...>;

/// @}

namespace detail {

struct branch_printer {
    branch_printer() { fmt::format_to(std::back_inserter(prefix), "  "); }

    template <class World, class FormatContext, class From, class To>
    auto operator()(const World&, FormatContext& ctx, From, To, bool last) const
    {
        fmt::format_to(
            ctx.out(),
            "{}{} {}\n",
            std::string_view(prefix.data(), prefix.size()),
            last ? "└─" : "├─",
            To{});
    }

    static constexpr auto mark(bool last) -> const char*
    {
        return last ? " " : "│";
    }

    auto ascend(bool last) &
    {
        const auto count = 2 + std::strlen(mark(last));
        prefix.resize(prefix.size() - count);
    }

    auto descend(bool last) &
    {
        fmt::format_to(std::back_inserter(prefix), "{}  ", mark(last));
    }

    fmt::memory_buffer prefix{};
};

struct orientation_printer {
    template <class World, class FormatContext, class From, class To>
    auto
    operator()(const World& world, FormatContext& ctx, From, To, bool) const
    {
        fmt::format_to(ctx.out(), "{}\n", world.template get<From, To>());
    }

    auto ascend(bool) & {}
    auto descend(bool) & {}
};

}  // namespace detail

}  // namespace turtle

template <turtle::kinematic::world W>
struct fmt::formatter<W> : fmt::formatter<typename W::scalar> {

    template <class Printer, class FormatContext, class From>
    auto print_tree(Printer&&, const W&, FormatContext&, From, metal::list<>)
    {}

    template <class Printer,
              class FormatContext,
              class From,
              turtle::kinematic::frame To,
              class... Frames>
    auto print_tree(Printer&& printer,
                    const W& world,
                    FormatContext& ctx,
                    From,
                    metal::list<To, Frames...>)
    {
        printer(world, ctx, From{}, To{}, sizeof...(Frames) == 0);
        print_tree(printer, world, ctx, From{}, metal::list<Frames...>{});
    }

    template <class Printer,
              class FormatContext,
              class From,
              class To,
              class... SubFrames,
              class... Frames>
    auto
    print_tree(Printer&& printer,
               const W& world,
               FormatContext& ctx,
               From,
               metal::list<turtle::meta::tree<To, SubFrames...>, Frames...>)
    {
        const auto last = sizeof...(Frames) == 0;
        printer(world, ctx, From{}, To{}, last);

        printer.descend(last);
        print_tree(printer, world, ctx, To{}, metal::list<SubFrames...>{});
        printer.ascend(last);

        print_tree(printer, world, ctx, From{}, metal::list<Frames...>{});
    }

    template <class FormatContext>
    auto format(const W& world, FormatContext& ctx)
    {
        auto&& out = ctx.out();
        const auto root = typename W::root{};

        format_to(out, "🐢 {}\n", root);

        print_tree(turtle::detail::branch_printer{},
                   world,
                   ctx,
                   root,
                   typename W::tree::branches{});
        print_tree(turtle::detail::orientation_printer{},
                   world,
                   ctx,
                   root,
                   typename W::tree::branches{});

        return out;
    }
};
