load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
load(
    ":configure.bzl",
    "configure_local_variables",
)

configure_local_variables(
    name = "local_config",
    defs_template = "//:defs.bzl.tpl",
)

http_archive(
  name = "bazel_clang_tidy",
  urls = ["https://github.com/erenon/bazel_clang_tidy/archive/1c3a983c054a41be73ac128e3d842a940850b5fe.zip"],
  strip_prefix = "bazel_clang_tidy-1c3a983c054a41be73ac128e3d842a940850b5fe",
  sha256 = "9943da5967e470a5c0165233242e51538b2a0fa11b15820d399314082cd99a09",
)

new_git_repository(
    name = "fmt",
    commit = "d9a731d4862891f274b63cef14bde0e5558a290f",
    remote = "https://github.com/fmtlib/fmt/",
    build_file = "@//:external/fmt.BUILD",
)

new_git_repository(
    name = "metal",
    commit = "8d687739ec9fbc7768f8cbf5e192d9ffdb41dc3a",
    remote = "https://github.com/brunocodutra/metal/",
    build_file = "@//:external/metal.BUILD",
)

new_git_repository(
    name = "ut",
    commit = "c3ed7a21a18a4de3fc807451ae86c9f51e706a11",
    remote = "https://github.com/boost-ext/ut/",
    build_file = "@//:external/ut.BUILD"
)

# https://github.com/bazelbuild/bazel/issues/8846
new_local_repository(
  name = "llvm",
  path = "/usr/lib/llvm-13/",
  build_file_content = """
package(default_visibility = ["//visibility:public"])
cc_library(
  name = "c++",
  hdrs = glob(["include/c++/v1/**/*"]),
  includes = [
    "include/c++/v1",
  ],
  strip_include_prefix = "include/c++/v1",
)
cc_library(
  name = "clang",
  hdrs = glob(["lib/clang/13.0.0/include/**/*"]),
  includes = [
    "lib/clang/13.0.0/include",
  ],
  strip_include_prefix = "lib/clang/13.0.0/include",
)
"""
)
