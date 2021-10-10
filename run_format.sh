#!/bin/bash

set -eux

format_bin=${1:-/usr/local/opt/llvm/bin/clang-format}
buildifier_bin=${2:-/usr/local/bin/buildifier}

find .  \( -name "*.cpp" -o -name "*.hpp" \) -exec ${format_bin} -i {} \;
find .  \( -name "*BUILD" -o -name "WORKSPACE" -o -name "*.bzl" \) | \
    xargs ${buildifier_bin}
