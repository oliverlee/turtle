load("@local_config//:defs.bzl", "PROJECT_DEFAULT_COPTS")
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
  name = "example",
  srcs = ["example.cpp"],
  copts = PROJECT_DEFAULT_COPTS,
  deps = ["//:turtle"],
)
