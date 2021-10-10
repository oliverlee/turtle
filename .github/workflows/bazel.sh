#!/bin/bash

set -eux

cache_hit=$1
os=$2
version=$3

if [[ "${cache_hit}" != 'true' ]]; then
    URL="https://github.com/bazelbuild/bazel/releases/download/${version}/bazel-${version}-installer-${os}-x86_64.sh"

    wget -O install.sh "${URL}"
    chmod +x install.sh
    ./install.sh --user
    rm -f install.sh
fi

sudo rm /usr/local/bin/bazel
sudo ln -s "${HOME}/bin/bazel" /usr/local/bin/bazel

