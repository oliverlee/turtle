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
    sha256 = "9943da5967e470a5c0165233242e51538b2a0fa11b15820d399314082cd99a09",
    strip_prefix = "bazel_clang_tidy-1c3a983c054a41be73ac128e3d842a940850b5fe",
    urls = ["https://github.com/erenon/bazel_clang_tidy/archive/1c3a983c054a41be73ac128e3d842a940850b5fe.zip"],
)

new_git_repository(
    name = "fmt",
    build_file = "@//:external/fmt.BUILD",
    commit = "274d10ed06d54d8c5c7b2a6072225e470c1e7c9f",
    remote = "https://github.com/oliverlee/fmt/",
)

new_git_repository(
    name = "metal",
    build_file = "@//:external/metal.BUILD",
    commit = "8d687739ec9fbc7768f8cbf5e192d9ffdb41dc3a",
    remote = "https://github.com/brunocodutra/metal/",
)

new_git_repository(
    name = "ut",
    build_file = "@//:external/ut.BUILD",
    commit = "c3ed7a21a18a4de3fc807451ae86c9f51e706a11",
    remote = "https://github.com/boost-ext/ut/",
)

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "rules_python",
    sha256 = "954aa89b491be4a083304a2cb838019c8b8c3720a7abb9c4cb81ac7a24230cea",
    url = "https://github.com/bazelbuild/rules_python/releases/download/0.4.0/rules_python-0.4.0.tar.gz",
)

load("@rules_python//python:pip.bzl", "pip_install")

pip_install(
    name = "pip",
    #python_interpreter_target = "@python_interpreter//:python_bin",
    requirements = "//doxygen:requirements.txt",
)

new_git_repository(
    name = "mcss",
    build_file = "@//:external/m.css.BUILD",
    commit = "9385194fa3392a7162e7535cc2478814e382ff8a",
    remote = "https://github.com/mosra/m.css/",
)
