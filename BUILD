load("@local_config//:defs.bzl", "PROJECT_DEFAULT_COPTS")

exports_files([".clang-tidy"], visibility=["//:__subpackages__"])

cc_library(
    name = "turtle",
    hdrs = [
        "include/turtle/turtle.hpp",
        "include/turtle/vector.hpp",
    ],
    copts = PROJECT_DEFAULT_COPTS,
    strip_include_prefix = "include",
    deps = ["@fmt"],
    visibility = ["//visibility:public"],
)
