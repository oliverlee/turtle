COMMON_CXX_WARN_OPTS = [
    "-Werror",
    "-Wall",
    "-Wextra",
    "-Wnon-virtual-dtor",
    "-Wold-style-cast",
    "-Wcast-align",
    "-Wunused",
    "-Woverloaded-virtual",
    "-Wpedantic",
    "-Wconversion",
    "-Wsign-conversion",
    "-Wdouble-promotion",
    "-Wformat=2",
]

CLANG_CXX_WARN_OPTS = [
    "-Wnull-dereference",
    "-fcolor-diagnostics",
]

GCC_CXX_WARN_OPTS = [
    "-Wshadow=compatible-local",
    "-Wlogical-op",
    "-Wuseless-cast",
    "-Wduplicated-cond",
    "-Wduplicated-branches",
    "-Wmisleading-indentation",
    "-fdiagnostics-color=always",
]

PROJECT_DEFAULT_COPTS = COMMON_CXX_WARN_OPTS + %{compiler_name}_CXX_WARN_OPTS
