#!/bin/bash

# Function to check if a package is installed
is_installed() {
    dpkg-query -l "$1" &>/dev/null || rpm -q "$1" &>/dev/null || pacman -Qs "$1" &>/dev/null || pkg info "$1" &>/dev/null
}

# Check the platform and install SDL3 accordingly
DISTRO=$(cat /etc/os-release | grep ^ID= | cut -d= -f2 | tr -d '"')

case $DISTRO in
"ubuntu" | "debian")
	# Debian/Ubuntu-based distributions: Install SDL3 via apt
	echo "Looks like Debian to me, using apt"
	sudo apt-get update
	sudo apt-get install -y libsdl3-dev
	echo "SDL3 installed"
	;;

"fedora")
	# Fedora: Install SDL3 via dnf
	echo "Looks like Fedora to me, using dnf"
	sudo dnf install -y libsdl3-devel
	echo "SDL3 installed"
	;;

"arch")
	# Arch: Install SDL3 via pacman
	echo "looks like Arch to me, using pacman"
	sudo pacman -Sy --noconfirm sdl3
	echo "Looks like Arch to me"
	echo "Installing SDL3..."
	;;

*)
	echo "Could not determine Linux distribution"
	echo "Supported: Ubuntu, Debian, Fedora, and Arch."
	exit 1
	;;
    esac
else
    echo "I have no clue what operating system this is, sorry"
    exit 1
fi

