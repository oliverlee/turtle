#!/bin/bash

format_bin=${1:-/usr/local/opt/llvm/bin/clang-format}
find .  \( -name "*.cpp" -o -name "*.hpp" \) -exec ${format_bin} -i {} \;
