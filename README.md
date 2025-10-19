# 🎨 Graphical Interface Programming Library in C# 🧩 ProjetC_IG — Graphical Interface Library in C



<div align="center">A lightweight, cross-platform GUI library written in C for building graphical user interfaces.

This project provides a complete toolkit for creating windows, widgets, drawing, and handling user interactions.

![License](https://img.shields.io/badge/license-MIT-blue.svg)

![Platform](https://img.shields.io/badge/platform-macOS%20%7C%20Linux%20%7C%20Windows-lightgrey)---

![Language](https://img.shields.io/badge/language-C-00599C.svg)

![CMake](https://img.shields.io/badge/CMake-3.20+-064F8C.svg)## 🧱 Tech Stack



A lightweight, cross-platform GUI programming library written in C for building graphical user interfaces from scratch.- Language: C

- Build system: CMake (min 3.20)

[Features](#-features) • [Installation](#-installation) • [Quick Start](#-quick-start) • [Documentation](#-documentation) • [Examples](#-examples)- Library layout: Static library `ei` + example executables

- Dependencies (linked in CMake): SDL2, SDL2_ttf, FreeImage, platform shim `eibase` from `./_macos`, `./_x11`, or `./_win`

</div>- Platforms: macOS, Linux (X11), Windows



------



## 📖 About## 🚀 Features



This project is a comprehensive graphical interface library implemented in C that provides a complete toolkit for creating windows, widgets, drawing primitives, and handling user interactions. Built with portability and performance in mind, it abstracts platform-specific details through SDL2 while offering a clean, widget-based API for GUI development.- Cross-platform backend (macOS, Windows, X11/Linux)

- Widget system (frames, buttons, custom widget classes)

### Key Highlights- Event handling for user interactions

- Geometry management via a placer

- 🎯 **Fully functional widget system** with frames, buttons, and extensible custom widgets- Low-level drawing primitives

- 🖼️ **Cross-platform support** for macOS, Linux (X11), and Windows- Extensible architecture (widget classes)

- 🎨 **Low-level drawing primitives** with relief effects and custom rendering

- ⚡ **Event-driven architecture** with comprehensive input handling---

- 📐 **Flexible geometry management** via placer system

- 🧩 **Modular design** with clear API/implementation separation## 📁 Project Structure



---```

.

## 🚀 Features├── api/            # Public API headers

├── implem/         # Library implementation (.c/.h)

### Core Components├── tests/          # Example apps (serve as manual tests)

├── _macos/         # macOS-specific binaries/sources (eibase)

- **Application Management** - Initialize, run, and manage the GUI application lifecycle├── _win/           # Windows-specific binaries/sources (eibase)

- **Widget System** - Hierarchical widget tree with frames, buttons, and custom widget classes├── _x11/           # X11/Linux-specific binaries/sources (eibase)

- **Event Handling** - Mouse, keyboard, and window events with callback support├── cmake/          # CMake build helpers (do out-of-source builds)

- **Drawing Engine** - Primitive shapes, text rendering, image support, and relief effects├── docs/           # Doxygen config & generated output

- **Layout Management** - Placer-based geometry management for flexible widget positioning├── misc/           # Miscellaneous files

- **Hardware Abstraction** - Platform-independent interface via SDL2 backend├── CMakeLists.txt  # Top-level build configuration

└── README.md

### Included Examples```



- **minimal** - Basic window with pixel manipulation---

- **hello_world** - Simple application setup

- **button** - Interactive button widgets## ✅ Requirements

- **frame** - Frame widget demonstrations

- **lines** - Drawing primitives showcase- CMake ≥ 3.20

- **la_souris_verte** - Mouse interaction demo- C compiler: Clang/GCC/MSVC

- **puzzle** - Interactive puzzle game- Runtime/libs:

- **two048** - 2048 game implementation  - SDL2, SDL2_ttf, FreeImage

- **minesweeper** - Complete minesweeper game  - Platform shim `eibase` (provided in repo under `_macos`, `_x11`, `_win`)



---Platform notes (as used by CMakeLists):

- macOS: headers expected under `/opt/local/include` and `/opt/local/include/SDL2`; libraries usually in `/opt/local/lib` (MacPorts). Xcode CLT required.

## 📁 Project Structure- Linux: headers under `/usr/include/SDL2`; link with `-lm`. Install packages e.g. on Debian/Ubuntu: `sudo apt-get install libsdl2-dev libsdl2-ttf-dev libfreeimage-dev`.

- Windows: CMake references `C:/projetc/SDL2_windows` for SDL2 and FreeImage.

```  - TODO: Document the exact Windows setup steps and provide prebuilt/links.

.

├── api/                    # 📚 Public API headers (DO NOT MODIFY)---

│   ├── ei_application.h    # Application lifecycle

│   ├── ei_widget.h         # Widget system## ⚙️ Configuration options

│   ├── ei_event.h          # Event handling

│   ├── ei_draw.h           # Drawing primitives- IG_ASAN: enable AddressSanitizer for debug runs.

│   ├── ei_placer.h         # Geometry management  - Use: `cmake -DIG_ASAN=ON ..` (Clang/GCC).

│   └── hw_interface.h      # Hardware abstraction

│---

├── implem/                 # 🔧 Library implementation

│   ├── ei_application.c## 🛠️ Build and Run

│   ├── ei_widget.c

│   ├── ei_draw.cOut-of-source builds are strongly recommended. You can use CLion (preferred) or plain CMake.

│   └── ...

│- CLion: open the project, select a target (e.g., `minimal`) and run/debug.

├── tests/                  # 🧪 Example applications- Plain CMake:

│   ├── minimal.c

│   ├── hello_world.c```bash

│   ├── button.c# Clone the repository

│   ├── puzzle.cgit clone <repo_url>

│   └── ...cd Graphical-Interface-User-Interface-Programming-Library-in-C

│

├── _macos/                 # 🍎 macOS platform binaries# Create build directory (outside source tree)

├── _x11/                   # 🐧 Linux/X11 platform binariesmkdir build && cd build

├── _win/                   # 🪟 Windows platform binaries

│# Configure

├── docs/                   # 📖 Documentationcmake -DCMAKE_BUILD_TYPE=Debug ..

│   ├── doxygen.cfg

│   └── html/# Build the library and example targets

│cmake --build . --target ig_all

├── CMakeLists.txt          # ⚙️ Build configuration

└── README.md# Or build a single example

```cmake --build . --target minimal



---# Run the executable (path from your build dir)

./minimal

## ✅ Requirements```



### Build ToolsNotes:

- A custom CMake target `ig_all` builds a subset of example apps: `minimal`, `frame`, `button`, `hello_world`, `puzzle`, `two048`, `minesweeper`.

- **CMake** ≥ 3.20- A custom target `doc` generates Doxygen documentation.

- **C Compiler** - Clang, GCC, or MSVC

- **Make** (or Ninja)---



### Dependencies## ▶️ Entry Points (Executables) and How to Run



- **SDL2** - Simple DirectMedia Layer 2All executables are defined under `tests/` and linked against the static library `ei`.

- **SDL2_ttf** - TrueType font renderingBuild any of them via `cmake --build <build_dir> --target <name>` then run `./<name>` from the build dir.

- **FreeImage** - Image loading and manipulation

- **eibase** - Platform-specific shim (included in repository)Executables:

- minimal

### Platform-Specific Setup- hello_world

- button

#### macOS- frame

- lines

```bash- dessin_relief

# Install dependencies via MacPorts- la_souris_verte

sudo port install libsdl2 libsdl2_ttf freeimage- puzzle

- two048

# Or via Homebrew- minesweeper

brew install sdl2 sdl2_ttf freeimage- test_d_sor3a

```- ext_testclass (links with `testclass` + `ei`)



Expected paths:Library:

- Headers: `/opt/local/include/SDL2` (MacPorts)- ei (static library built from `implem/*.c`)

- Libraries: `/opt/local/lib`

---

#### Linux (Ubuntu/Debian)

## 📚 API Overview (Headers in `api/`)

```bash

sudo apt-get update- ei_application.h — Application lifecycle management

sudo apt-get install libsdl2-dev libsdl2-ttf-dev libfreeimage-dev build-essential cmake- ei_widget.h — Widget creation and hierarchy

```- ei_widget_configure.h — Widget configuration

- ei_event.h — Event handling system

#### Windows- ei_draw.h — Drawing primitives

- ei_placer.h — Geometry management

1. Download SDL2 development libraries from [libsdl.org](https://www.libsdl.org/)- ei_types.h — Common type definitions

2. Download FreeImage from [freeimage.sourceforge.net](http://freeimage.sourceforge.net/)- hw_interface.h — Hardware abstraction layer

3. Place libraries in `C:/projetc/SDL2_windows/` (or update `CMakeLists.txt`)

---

---

## 🧪 Tests

## 🛠️ Installation

There is no separate unit test framework in this repository. The example applications under `tests/` act as manual/system tests. Typical quick checks:

### Using CLion (Recommended)

```bash

1. **Clone the repository**cmake --build <build_dir> --target minimal && <build_dir>/minimal

   ```bashcmake --build <build_dir> --target hello_world && <build_dir>/hello_world

   git clone https://github.com/m871-akram/Graphical-Interface-User-Interface-Programming-Library-in-C.gitcmake --build <build_dir> --target button && <build_dir>/button

   cd Graphical-Interface-User-Interface-Programming-Library-in-C```

   ```

If using CLion, select the target and press Run.

2. **Open in CLion**

   - Open CLion---

   - Select "Open" and navigate to the project directory

   - CLion will automatically configure CMake## 📖 Documentation



3. **Select and run a target**Generate the API documentation using Doxygen (config at `docs/doxygen.cfg`). From project root:

   - Choose a target from the dropdown (e.g., `minimal`, `hello_world`)

   - Click the Run button (▶️) or press `Shift + F10````bash

cmake --build <build_dir> --target doc

### Using Command Line# or directly

( cd docs && doxygen doxygen.cfg )

```bash```

# Clone the repository

git clone https://github.com/m871-akram/Graphical-Interface-User-Interface-Programming-Library-in-C.gitOutput will be available at:

cd Graphical-Interface-User-Interface-Programming-Library-in-C- docs/html/index.html (HTML)

- docs/latex (LaTeX)

# Create a build directory

mkdir build && cd build---



# Configure with CMake## 🔧 Environment Variables

cmake -DCMAKE_BUILD_TYPE=Debug ..

Currently, no runtime environment variables are required by the library itself.

# Build all examples

cmake --build . --target ig_allPotential platform requirements:

- Linux/X11 may require a running X server.

# Or build a specific target- macOS may require allowing the app to access the display.

cmake --build . --target minimal

Build-time options are provided via CMake cache variables (see IG_ASAN above).

# Run an example

./minimal---

```

## 🤝 Contributing

### Build Options

Contributions are welcome. Please:

Enable **AddressSanitizer** for debugging memory issues:- Follow the existing C coding style

- Keep code portable across macOS/Linux/Windows

```bash- Update or add example apps to demonstrate new features

cmake -DCMAKE_BUILD_TYPE=Debug -DIG_ASAN=ON ..- Update documentation comments for public APIs

cmake --build .

```---



---## 📎 Notes for Building



## 🚀 Quick Start- Prefer out-of-source builds to keep the repository clean.

- If you use CLion, it manages CMake generation and build directories for you.

### Minimal Example- Legacy note (FR): Évitez d’exécuter cmake depuis la racine du dépôt car cela peut générer beaucoup de fichiers. Utilisez un répertoire de build séparé (p. ex. `build/`) ou laissez CLion s’en charger. Le répertoire `cmake/` contient des fichiers auxiliaires pour CMake.



Here's a simple example that creates a window and fills it with colors:---



```c

#include "ei_types.h"## 🔗 Additional Resources

#include "hw_interface.h"

- Doxygen config: `docs/doxygen.cfg`

int main(int argc, char* argv[]) {- Example sources: `tests/`

    ei_surface_t main_window = NULL;- Public headers: `api/`

    ei_size_t size = ei_size(640, 480);- Implementation sources: `implem/`

    

    // Initialize hardware
    hw_init();
    
    // Create window
    main_window = hw_create_window(size, false);
    
    // Draw and display
    hw_surface_lock(main_window);
    // ... drawing code ...
    hw_surface_unlock(main_window);
    hw_surface_update_rects(main_window, NULL);
    
    // Event loop
    ei_event_t event;
    event.type = ei_ev_none;
    while (event.type != ei_ev_close && event.type != ei_ev_keydown)
        hw_event_wait_next(&event);
    
    // Cleanup
    hw_surface_free(main_window);
    hw_quit();
    
    return 0;
}
```

### Running Examples

```bash
# Build and run the minimal example
cmake --build build --target minimal && ./build/minimal

# Build and run the button demo
cmake --build build --target button && ./build/button

# Build and run the puzzle game
cmake --build build --target puzzle && ./build/puzzle

# Build and run minesweeper
cmake --build build --target minesweeper && ./build/minesweeper
```

---

## 📚 API Overview

### Application Management (`ei_application.h`)

```c
void ei_app_create(ei_size_t main_window_size, bool fullscreen);
void ei_app_run(void);
void ei_app_free(void);
ei_surface_t ei_app_root_surface(void);
ei_widget_t ei_app_root_widget(void);
```

### Widget System (`ei_widget.h`, `ei_widget_configure.h`)

- Create and manage widget hierarchy
- Configure widget properties (size, color, text, callbacks)
- Support for frames, buttons, and custom widget classes

### Event Handling (`ei_event.h`)

- Mouse events (click, move, drag)
- Keyboard events
- Window events (close, expose)
- Custom event callbacks

### Drawing Primitives (`ei_draw.h`)

- Lines, polylines, polygons
- Rectangles, circles, arcs
- Text rendering
- Image blitting

### Layout Management (`ei_placer.h`)

- Anchor-based positioning
- Relative and absolute sizing
- Flexible geometry management

---

## 📖 Documentation

### Generate API Documentation

The project uses Doxygen for API documentation:

```bash
# Generate from CMake
cmake --build build --target doc

# Or directly with Doxygen
cd docs
doxygen doxygen.cfg
```

Documentation will be available at:
- **HTML**: `docs/html/index.html`
- **LaTeX**: `docs/latex/`

### Reading the Code

- Start with `api/` headers for public interface
- Review `tests/` for usage examples
- Explore `implem/` for implementation details

---

## 🎮 Examples

### Available Test Applications

| Application | Description | Features |
|------------|-------------|----------|
| `minimal` | Basic pixel manipulation | Surface locking, direct pixel access |
| `hello_world` | Simple application setup | Basic window creation |
| `lines` | Drawing primitives | Lines, shapes, colors |
| `dessin_relief` | Relief effects | 3D-style borders and effects |
| `frame` | Frame widgets | Container widgets, hierarchy |
| `button` | Interactive buttons | Event callbacks, user interaction |
| `la_souris_verte` | Mouse demo | Mouse tracking and events |
| `puzzle` | Sliding puzzle game | Complete interactive application |
| `two048` | 2048 game | Game logic, grid layout |
| `minesweeper` | Minesweeper game | Advanced UI, game state management |

### Running Specific Examples

#### CLion
1. Select target from dropdown menu
2. Click Run (▶️) or Debug (🐛)

#### Command Line
```bash
# Using CMake build system
cd build
cmake --build . --target <target_name>
./<target_name>

# Example: Run the puzzle game
cmake --build . --target puzzle
./puzzle
```

---

## 🧪 Testing

The project uses example applications as integration tests. No separate unit test framework is included.

### Quick Test Suite

```bash
cd build

# Test basic functionality
./minimal

# Test widgets
./frame
./button

# Test games (comprehensive tests)
./puzzle
./two048
./minesweeper
```

---

## 🔧 Advanced Configuration

### CMake Options

- `CMAKE_BUILD_TYPE` - Build configuration (Debug, Release, RelWithDebInfo)
- `IG_ASAN` - Enable AddressSanitizer (ON/OFF)

### Custom Build Targets

- `ig_all` - Builds all main examples (minimal, frame, button, hello_world, puzzle, two048, minesweeper)
- `doc` - Generates Doxygen documentation
- Individual targets for each example application

---

## 🤝 Contributing

Contributions are welcome! Please follow these guidelines:

### Code Style

- Follow existing C coding conventions
- Use 8-space tabs (as specified in CMakeLists.txt)
- Keep code portable across platforms
- Document public APIs with Doxygen comments

### Contribution Process

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Implement your changes
4. Test on multiple platforms if possible
5. Commit your changes (`git commit -m 'Add amazing feature'`)
6. Push to the branch (`git push origin feature/amazing-feature`)
7. Open a Pull Request

### Adding New Features

- Add example applications to `tests/` to demonstrate features
- Update API headers in `api/` (coordinate with maintainers)
- Implement functionality in `implem/`
- Update documentation and README

---

## 🐛 Troubleshooting

### Common Issues

**CMake can't find SDL2**
```bash
# Linux
sudo apt-get install libsdl2-dev

# macOS (MacPorts)
sudo port install libsdl2

# macOS (Homebrew)
brew install sdl2
```

**Linker errors with eibase**
- Ensure the correct platform directory is selected (_macos, _x11, or _win)
- Check that `eibase` library exists in the platform directory

**Window doesn't appear (Linux)**
- Ensure X11 server is running
- Check `DISPLAY` environment variable: `echo $DISPLAY`

**AddressSanitizer errors**
- Build without ASAN first to verify functionality
- Review memory allocation/deallocation in your code

---

## 📋 Build Notes

### Out-of-Source Builds

Always use out-of-source builds to keep the repository clean:

```bash
# ✅ Good - out-of-source build
mkdir build && cd build
cmake ..

# ❌ Bad - in-source build
cmake .  # Don't do this!
```

### CLion Configuration

CLion automatically manages build directories. Default locations:
- Debug builds: `cmake-build-debug/`
- Release builds: `cmake-build-release/`

### Platform-Specific Notes

**macOS**
- Requires Xcode Command Line Tools: `xcode-select --install`
- May need to grant accessibility permissions for window creation

**Linux**
- Requires X11 development headers
- May need to install `build-essential` package

**Windows**
- MSVC compiler recommended
- Adjust library paths in `CMakeLists.txt` if needed

---

## 📄 License

This project is part of an educational program. Please refer to your institution's policies regarding code usage and distribution.

---

## 🙏 Acknowledgments

- Original framework by **François Bérard** (Ensimag)
- Built on **SDL2** and **FreeImage** libraries
- Inspired by classic GUI programming paradigms

---

## 📞 Contact & Support

- **Repository**: [GitHub](https://github.com/m871-akram/Graphical-Interface-User-Interface-Programming-Library-in-C)
- **Issues**: [GitHub Issues](https://github.com/m871-akram/Graphical-Interface-User-Interface-Programming-Library-in-C/issues)

---

<div align="center">

**Made with ❤️ using C and SDL2**

⭐ Star this repository if you find it helpful!

</div>
