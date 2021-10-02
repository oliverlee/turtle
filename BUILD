load(
  "@local_config//:defs.bzl",
  "PROJECT_DEFAULT_COPTS",
  "PROJECT_LLVM_WORKAROUND_DEPS",
)

exports_files([".clang-tidy"], visibility=["//:__subpackages__"])

cc_library(
    name = "turtle",
    hdrs = [
        "include/turtle/frame.hpp",
        "include/turtle/fwd.hpp",
        "include/turtle/meta.hpp",
        "include/turtle/orientation.hpp",
        "include/turtle/quaternion.hpp",
        "include/turtle/turtle.hpp",
        "include/turtle/util/array.hpp",
        "include/turtle/util/ulp_diff.hpp",
        "include/turtle/util/zip_transform_iterator.hpp",
        "include/turtle/vector.hpp",
        "include/turtle/vector_ops.hpp",
        "include/turtle/world.hpp",
    ],
    copts = PROJECT_DEFAULT_COPTS,
    strip_include_prefix = "include",
    deps = ["@fmt"] + PROJECT_LLVM_WORKAROUND_DEPS,
    visibility = ["//visibility:public"],
)
