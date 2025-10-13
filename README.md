# 🧩 ProjetC_IG — Graphical Interface Library in C

A lightweight, cross-platform GUI library written in C for building graphical user interfaces.  
This project provides a complete toolkit for creating windows, widgets, and handling user interactions.

---

## 🚀 Features

- **Cross-Platform Support:** Native implementations for macOS, Windows, and X11 (Linux)
- **Widget System:** Comprehensive set of UI widgets (frames, buttons, and custom widgets)
- **Event Handling:** Complete event management system for user interactions
- **Geometry Management:** Placer-based layout management for flexible UI design
- **Drawing Primitives:** Low-level drawing API for custom graphics
- **Extensible Architecture:** Plugin system for defining custom widget classes

---

## 📁 Project Structure

```
.
├── api/           # Public API headers
├── implem/        # Implementation files
├── tests/         # Test programs and examples
├── _macos/        # macOS-specific code
├── _win/          # Windows-specific code
├── _x11/          # X11/Linux-specific code
├── cmake/         # CMake build configuration
├── docs/          # Documentation (Doxygen)
└── misc/          # Miscellaneous files
```

---

## 🛠️ Building the Project

### Prerequisites

- **CMake** ≥ 3.10
- **C Compiler:** GCC, Clang, or MSVC

#### Platform-specific dependencies

- **Linux:** X11 development libraries
- **macOS:** Xcode Command Line Tools
- **Windows:** Visual Studio or MinGW

### Build Instructions

```bash
# Clone the repository
git clone <repo_url>
cd ProjetC_IG

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
cmake --build .
```

---

## 📚 API Overview

The library provides several main components:

| Header | Description |
|--------|--------------|
| `ei_application.h` | Application lifecycle management |
| `ei_widget.h` | Widget creation and hierarchy |
| `ei_widget_configure.h` | Widget configuration |
| `ei_event.h` | Event handling system |
| `ei_draw.h` | Drawing primitives |
| `ei_placer.h` | Geometry management |
| `ei_types.h` | Common type definitions |
| `hw_interface.h` | Hardware abstraction layer |

---

## 🎯 Example Programs

Example programs can be found in the `tests/` directory:

- `minimal.c` — Minimal GUI application
- `hello_world.c` — Hello World example
- `button.c` — Button widget demonstration
- `frame.c` — Frame widget examples
- `puzzle.c` — Puzzle game
- `two048.c` — 2048 game implementation
- `minesweeper.c` — Minesweeper game

---

## 🚦 Quick Start

Here’s a minimal example to get started:

```c
#include "ei_application.h"
#include "ei_widget_configure.h"

int main(int argc, char* argv[]) {
    ei_app_create((ei_size_t){600, 400}, false);
    ei_surface_t root_surface = ei_app_root_surface();
    ei_widget_t root_widget = ei_app_root_widget();

    // Your GUI code here

    ei_app_run();
    ei_app_free();
    return 0;
}
```

---

## 📖 Documentation

To generate the full API documentation:

```bash
cd docs
doxygen doxygen.cfg
```

Documentation will be available at:  
👉 `docs/html/index.html`

---

## 🧪 Running Tests

```bash
cd build
./minimal
./hello_world
./button
```

---

## 🤝 Contributing

Contributions are welcome!  
Please ensure your code:

- Follows the existing style conventions
- Includes appropriate tests



## 🔗 Additional Resources

- See [`api/README`](api/README) for detailed API documentation
- See [`tests/README`](tests/README) for information about test programs
- See [`implem/README`](implem/README) for implementation details
- See [`docs/README`](docs/README) for documentation generation instructions

---

## 🐛 Known Issues

Please check the issue tracker for known bugs and feature requests.
> **Note:** This is an educational project for learning GUI programming in C.

Ne lancez pas cmake directement depuis ce répertoire car cmake génère beaucoup de fichiers. Lancez cmake depuis le répertoire "cmake", ou mieux, construisez votre projet avec CLion (qui s'occupe d'appeler cmake): lisez clion/README.
