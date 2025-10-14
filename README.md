# 🧩 ProjetC_IG — Graphical Interface Library in C

A lightweight, cross-platform GUI library written in C for building graphical user interfaces.
This project provides a complete toolkit for creating windows, widgets, drawing, and handling user interactions.

---

## 🧱 Tech Stack

- Language: C
- Build system: CMake (min 3.20)
- Library layout: Static library `ei` + example executables
- Dependencies (linked in CMake): SDL2, SDL2_ttf, FreeImage, platform shim `eibase` from `./_macos`, `./_x11`, or `./_win`
- Platforms: macOS, Linux (X11), Windows

---

## 🚀 Features

- Cross-platform backend (macOS, Windows, X11/Linux)
- Widget system (frames, buttons, custom widget classes)
- Event handling for user interactions
- Geometry management via a placer
- Low-level drawing primitives
- Extensible architecture (widget classes)

---

## 📁 Project Structure

```
.
├── api/            # Public API headers
├── implem/         # Library implementation (.c/.h)
├── tests/          # Example apps (serve as manual tests)
├── _macos/         # macOS-specific binaries/sources (eibase)
├── _win/           # Windows-specific binaries/sources (eibase)
├── _x11/           # X11/Linux-specific binaries/sources (eibase)
├── cmake/          # CMake build helpers (do out-of-source builds)
├── docs/           # Doxygen config & generated output
├── misc/           # Miscellaneous files
├── CMakeLists.txt  # Top-level build configuration
└── README.md
```

---

## ✅ Requirements

- CMake ≥ 3.20
- C compiler: Clang/GCC/MSVC
- Runtime/libs:
  - SDL2, SDL2_ttf, FreeImage
  - Platform shim `eibase` (provided in repo under `_macos`, `_x11`, `_win`)

Platform notes (as used by CMakeLists):
- macOS: headers expected under `/opt/local/include` and `/opt/local/include/SDL2`; libraries usually in `/opt/local/lib` (MacPorts). Xcode CLT required.
- Linux: headers under `/usr/include/SDL2`; link with `-lm`. Install packages e.g. on Debian/Ubuntu: `sudo apt-get install libsdl2-dev libsdl2-ttf-dev libfreeimage-dev`.
- Windows: CMake references `C:/projetc/SDL2_windows` for SDL2 and FreeImage.
  - TODO: Document the exact Windows setup steps and provide prebuilt/links.

---

## ⚙️ Configuration options

- IG_ASAN: enable AddressSanitizer for debug runs.
  - Use: `cmake -DIG_ASAN=ON ..` (Clang/GCC).

---

## 🛠️ Build and Run

Out-of-source builds are strongly recommended. You can use CLion (preferred) or plain CMake.

- CLion: open the project, select a target (e.g., `minimal`) and run/debug.
- Plain CMake:

```bash
# Clone the repository
git clone <repo_url>
cd Graphical-Interface-User-Interface-Programming-Library-in-C

# Create build directory (outside source tree)
mkdir build && cd build

# Configure
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Build the library and example targets
cmake --build . --target ig_all

# Or build a single example
cmake --build . --target minimal

# Run the executable (path from your build dir)
./minimal
```

Notes:
- A custom CMake target `ig_all` builds a subset of example apps: `minimal`, `frame`, `button`, `hello_world`, `puzzle`, `two048`, `minesweeper`.
- A custom target `doc` generates Doxygen documentation.

---

## ▶️ Entry Points (Executables) and How to Run

All executables are defined under `tests/` and linked against the static library `ei`.
Build any of them via `cmake --build <build_dir> --target <name>` then run `./<name>` from the build dir.

Executables:
- minimal
- hello_world
- button
- frame
- lines
- dessin_relief
- la_souris_verte
- puzzle
- two048
- minesweeper
- test_d_sor3a
- ext_testclass (links with `testclass` + `ei`)

Library:
- ei (static library built from `implem/*.c`)

---

## 📚 API Overview (Headers in `api/`)

- ei_application.h — Application lifecycle management
- ei_widget.h — Widget creation and hierarchy
- ei_widget_configure.h — Widget configuration
- ei_event.h — Event handling system
- ei_draw.h — Drawing primitives
- ei_placer.h — Geometry management
- ei_types.h — Common type definitions
- hw_interface.h — Hardware abstraction layer

---

## 🧪 Tests

There is no separate unit test framework in this repository. The example applications under `tests/` act as manual/system tests. Typical quick checks:

```bash
cmake --build <build_dir> --target minimal && <build_dir>/minimal
cmake --build <build_dir> --target hello_world && <build_dir>/hello_world
cmake --build <build_dir> --target button && <build_dir>/button
```

If using CLion, select the target and press Run.

---

## 📖 Documentation

Generate the API documentation using Doxygen (config at `docs/doxygen.cfg`). From project root:

```bash
cmake --build <build_dir> --target doc
# or directly
( cd docs && doxygen doxygen.cfg )
```

Output will be available at:
- docs/html/index.html (HTML)
- docs/latex (LaTeX)

---

## 🔧 Environment Variables

Currently, no runtime environment variables are required by the library itself.

Potential platform requirements:
- Linux/X11 may require a running X server.
- macOS may require allowing the app to access the display.

Build-time options are provided via CMake cache variables (see IG_ASAN above).

---

## 🤝 Contributing

Contributions are welcome. Please:
- Follow the existing C coding style
- Keep code portable across macOS/Linux/Windows
- Update or add example apps to demonstrate new features
- Update documentation comments for public APIs

---

## 📎 Notes for Building

- Prefer out-of-source builds to keep the repository clean.
- If you use CLion, it manages CMake generation and build directories for you.
- Legacy note (FR): Évitez d’exécuter cmake depuis la racine du dépôt car cela peut générer beaucoup de fichiers. Utilisez un répertoire de build séparé (p. ex. `build/`) ou laissez CLion s’en charger. Le répertoire `cmake/` contient des fichiers auxiliaires pour CMake.

---


## 🔗 Additional Resources

- Doxygen config: `docs/doxygen.cfg`
- Example sources: `tests/`
- Public headers: `api/`
- Implementation sources: `implem/`

