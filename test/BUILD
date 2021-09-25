load("@local_config//:defs.bzl", "PROJECT_DEFAULT_COPTS")
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

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
