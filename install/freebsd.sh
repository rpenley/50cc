#!/bin/bash
if [[ "$(uname -s)" == "FreeBSD" ]]; then
    # FreeBSD: Install SDL3 via pkg
    echo "Looks like FreeBSD to me, using pkg"
    sudo pkg install -y sdl3 pkg-config
    echo "installed SDL3 and pkg-config"
else 
	echo "Doesn't look like FreeBSD to me, sorry"
