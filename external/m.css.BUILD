load("@rules_python//python:defs.bzl", "py_binary")
load("@pip//:requirements.bzl", "requirement")

py_binary(
    name = "doxygen",
    srcs = glob(["**/*.py"]),
    args = ["doxygen/Doxyfile-mcss"],
    data = ["@//doxygen:config"] + glob([
        "**/*.js",
    ]) + glob([
        "**/*.png",
    ]) + glob([
        "**/*.css",
    ]) + glob([
        "**/*.html",
    ]) + ["@//:headers"],
    main = "documentation/doxygen.py",
    deps = [
        requirement("jinja2"),
        requirement("pygments"),
    ],
)
