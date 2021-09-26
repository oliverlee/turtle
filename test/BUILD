load("@local_config//:defs.bzl", "PROJECT_DEFAULT_COPTS")
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

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
  name = "vector",
  size = "small",
  srcs = ["vector.cpp"],
  copts = PROJECT_DEFAULT_COPTS,
  deps = [
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
