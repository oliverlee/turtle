# turtle
A library for handling reference frames

#### linting
Linting is a config

    $ bazel build --config=clang-tidy //...

This may not work on macOS (possibly due to
[this](https://github.com/bazelbuild/bazel/issues/12049)).

You can take the very annoying step of modifying
`bazel-exclusive/external/bazel_clang_tidy/clang_tidy/run_clang_tidy.sh` and
hard-coding the path to `clang-tidy`. Or create a symlink in one directory of
Bazel's macOS default shell env (e.g. `/usr/local/bin`). You'll also need Bazel
to detect that Clang is the CXX compiler, otherwise a bunch of GCC flags get
used. There's probably a better way to do it but I'm not a Bazel expert.
