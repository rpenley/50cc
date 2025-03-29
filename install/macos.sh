#!/bin/bash

if [[ "$(uname -s)" == "Darwin" ]]; then
    echo "Looks like a Mac to me, using brew"
    brew install sdl3 pkg-config
    echo "SDL3 and pkg-config installed on macOS."
else
    echo "This does not look like MacOS"
    exit 1
fi

