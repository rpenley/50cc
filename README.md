# 50cc

50cc is a minimalist 2D video game engine sandbox written in a single file of C99 with SDL3 as its only dependency.

50cc allows you to learn, explore and play several 2D games from a series of simple 2D engine primatives and functions. The entire engine and games fit in one C file and a simple/optional text based config, making it easy to modify, extend, or use as a learning tool.

![50cc Logo](docs/images/logo.png) <!-- You may want to add a logo image -->

## Why "50cc"?

The name "50cc" is inspired by small, efficient engines that power entry-level motorcycles and scooters. Just like those engines, this project aims to be:

1. **Lightweight** - The entire codebase is small and focused
2. **Accessible** - Easy to understand and modify
3. **Fun** - Despite its simplicity, it provides genuine entertainment

## Why C?

- Good performance on most hardware
- Cross platform compatibility
- Direct access to SDL3
- Less abstration so you can see how everything works
- Compiles quickly and only depends on SDL3 and standard libraries

### Prerequisites

- A C99 compatible compiler (GCC, Clang, MSVC)
- SDL3 library
- CMake (3.16 or newer)
- pkg-config (On Unix-Like)

### Supported Platforms

- Arch Linux
- Ubuntu/Debian
- macOS
- Windows
- FreeBSD

### Installation

Use the appropriate installation script from the `Install` folder:

```bash
# Linux
./Install/linux.sh

# MacOS
./Install/macos.sh

# Windows
Install\windows.ps
```


2. Build using CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

3. Run the game:
   ```bash
   ./50cc
   ```

## Usage

Once launched, 50cc will present a menu to select from the available games:

- Use arrow keys to navigate
- Press Enter/Return to select a game
- Press Esc to return to the menu
- Each game has its own controls, shown on-screen

## Customization

The entire game is contained in a single `50cc.c` file, making it easy to modify:

1. Open `50cc.c` in your favorite text editor
2. Modify the code as desired
3. Rebuild using the steps above
4. Enjoy your customized version!

## Why 50cc? (The Engine)

Just like this project, 50cc combustion engines have several merits:

- **Efficiency**: 50cc engines can achieve excellent fuel economy, often 100+ MPG
- **Simplicity**: Fewer moving parts means less that can go wrong
- **Accessibility**: Lower cost of entry and maintenance
- **Legality**: In many regions, 50cc vehicles can be operated without a full motorcycle license
- **Learning**: Perfect for beginners to understand the principles of internal combustion
- **Reliability**: When properly maintained, can run for thousands of miles with minimal issues
- **Portability**: Lightweight and compact design
- **Low environmental impact**: Smaller carbon footprint compared to larger engines

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- SDL3 developers for the excellent cross-platform library
- Inspiration from classic 2D games that defined the industry
- The open-source community for resources and documentation
