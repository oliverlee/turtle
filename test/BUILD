load("@local_config//:defs.bzl", "PROJECT_DEFAULT_COPTS")
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "util",
    testonly = True,
    hdrs = ["util/within.hpp"],
    copts = PROJECT_DEFAULT_COPTS,
    visibility = ["//turtle/test:__pkg__"],
    deps = [
        "//:turtle",
        "@ut",
    ],
)

cc_test(
    name = "formatter",
    size = "small",
    srcs = ["formatter.cpp"],
    copts = PROJECT_DEFAULT_COPTS,
    deps = [
        "//:turtle",
        "@fmt",
        "@ut",
    ],
)

cc_test(
    name = "frame",
    size = "small",
    srcs = ["frame.cpp"],
    copts = PROJECT_DEFAULT_COPTS,
    deps = [
        "//:turtle",
        "@ut",
    ],
)

cc_test(
    name = "meta",
    size = "small",
    srcs = ["meta.cpp"],
    copts = PROJECT_DEFAULT_COPTS,
    deps = [
        "//:turtle",
        "@ut",
    ],
)

cc_test(
    name = "orientation",
    size = "small",
    srcs = ["orientation.cpp"],
    copts = PROJECT_DEFAULT_COPTS,
    deps = [
        ":util",
        "//:turtle",
        "@ut",
    ],
)

cc_test(
    name = "point",
    size = "small",
    srcs = ["point.cpp"],
    copts = PROJECT_DEFAULT_COPTS,
    deps = [
        ":util",
        "//:turtle",
        "@ut",
    ],
)

cc_test(
    name = "quaternion",
    size = "small",
    srcs = ["quaternion.cpp"],
    copts = PROJECT_DEFAULT_COPTS,
    deps = [
        ":util",
        "//:turtle",
        "@ut",
    ],
)

cc_test(
    name = "vector",
    size = "small",
    srcs = ["vector.cpp"],
    copts = PROJECT_DEFAULT_COPTS,
    deps = [
        ":util",
        "//:turtle",
        "@ut",
    ],
)

cc_test(
    name = "vector_ops",
    size = "small",
    srcs = ["vector_ops.cpp"],
    copts = PROJECT_DEFAULT_COPTS,
    deps = [
        "//:turtle",
        "@ut",
    ],
)

cc_test(
    name = "world",
    size = "small",
    srcs = ["world.cpp"],
    copts = PROJECT_DEFAULT_COPTS,
    deps = [
        ":util",
        "//:turtle",
        "@ut",
    ],
)
