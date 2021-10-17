load(
    "@local_config//:defs.bzl",
    "PROJECT_DEFAULT_COPTS",
)

exports_files(
    [".clang-tidy"],
    visibility = ["//:__subpackages__"],
)

filegroup(
    name = "headers",
    srcs = [
        "include/turtle/frame.hpp",
        "include/turtle/fwd.hpp",
        "include/turtle/meta.hpp",
        "include/turtle/orientation.hpp",
        "include/turtle/point.hpp",
        "include/turtle/position.hpp",
        "include/turtle/quaternion.hpp",
        "include/turtle/turtle.hpp",
        "include/turtle/util/array.hpp",
        "include/turtle/util/ulp_diff.hpp",
        "include/turtle/util/zip_transform_iterator.hpp",
        "include/turtle/vector.hpp",
        "include/turtle/vector_interface.hpp",
        "include/turtle/vector_ops.hpp",
        "include/turtle/velocity.hpp",
        "include/turtle/world.hpp",
    ],
    visibility = ["@mcss//:__pkg__"],
)

cc_library(
    name = "turtle",
    hdrs = [":headers"],
    copts = PROJECT_DEFAULT_COPTS,
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = [
        "@fmt",
        "@metal",
    ],
)
