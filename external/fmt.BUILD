load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "fmt",
    hdrs = glob(["include/**/*"]),
    defines = ["FMT_HEADER_ONLY"],
    includes = ["include"],
    visibility = ["//visibility:public"],
)
