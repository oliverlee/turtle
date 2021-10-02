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
    name = "ut",
    commit = "c3ed7a21a18a4de3fc807451ae86c9f51e706a11",
    remote = "https://github.com/boost-ext/ut/",
    build_file = "@//:external/ut.BUILD"
)
