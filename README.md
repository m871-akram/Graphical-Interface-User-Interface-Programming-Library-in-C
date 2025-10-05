
# ProjetC_IG - Graphical Interface User Interface Programming Library in C

A comprehensive C-based graphical user interface library for building interactive applications with widgets, event handling, and drawing capabilities.

## 📋 Overview

ProjetC_IG is a GUI toolkit written in C that provides a foundation for creating graphical applications. The library includes support for various widgets (frames, buttons, etc.), drawing primitives, event handling, and geometry management through a placer system.

## 🚀 Features

- **Widget System**: Hierarchical widget framework with support for custom widget classes
- **Drawing API**: Low-level drawing primitives for lines, shapes, and graphics
- **Event Management**: Comprehensive event handling system for user interactions
- **Layout Management**: Placer-based geometry management for widget positioning
- **Cross-Platform**: Supports Linux, macOS, and Windows

## 🛠️ Dependencies

The project requires the following libraries:

- **SDL2**: Simple DirectMedia Layer for windowing and input
- **SDL2_ttf**: TrueType font rendering
- **FreeImage**: Image loading and processing

## 📦 Project Structure

```
ProjetC_IG/
├── api/                    # Public API header files
│   ├── ei_application.h    # Application management
│   ├── ei_draw.h          # Drawing functions
│   ├── ei_event.h         # Event handling
│   ├── ei_types.h         # Core types and structures
│   ├── ei_widget.h        # Widget base functionality
│   ├── ei_widgetclass.h   # Widget class management
│   ├── ei_placer.h        # Geometry management
│   └── hw_interface.h     # Hardware abstraction layer
├── implem/                # Implementation files
│   ├── ei_draw.c
│   └── ei_implementation.c
├── tests/                 # Test programs and examples
├── docs/                  # Documentation
├── _macos/               # macOS platform files
├── _x11/                 # Linux platform files
└── _win/                 # Windows platform files
```


## 🔧 Building the Project

### Prerequisites

#### Linux
```shell script
sudo apt-get install libsdl2-dev libsdl2-ttf-dev libfreeimage-dev
```


#### macOS
```shell script
port install SDL2 SDL2_ttf freeimage
```


#### Windows
Set up SDL2 libraries in `C:/projetc/SDL2_windows/`

### Build Instructions

```shell script
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .

# Build all targets
cmake --build . --target ig_all

# Generate documentation
cmake --build . --target doc
```


### Optional: Address Sanitizer

To enable Address Sanitizer for debugging:
```shell script
cmake -DIG_ASAN=ON ..
```


## 🎮 Demo Applications

The project includes several demonstration programs:

### Basic Examples
- **minimal**: Minimal SDL2 window setup
- **lines**: Line drawing demonstration
- **dessin_relief**: 3D relief drawing example
- **frame**: Frame widget demonstration
- **button**: Button widget demonstration
- **hello_world**: Simple "Hello World" GUI application

### Games
- **puzzle**: Interactive puzzle game
- **two048**: 2048 game implementation
- **minesweeper**: Classic minesweeper game

### Testing
- **test_d_sor3a**: Performance testing
- **ext_testclass**: Widget class extension testing

## 🎯 Usage Example

```c++
#include "ei_application.h"
#include "ei_widget.h"

int main(int argc, char* argv[]) {
    ei_app_create((ei_size_t){800, 600}, EI_FALSE);
    
    // Create and configure widgets
    // ... your code here ...
    
    ei_app_run();
    ei_app_free();
    return 0;
}
```


## 📚 Documentation

Documentation can be generated using Doxygen:

```shell script
cmake --build . --target doc
```


The generated documentation will be available in the `docs/` directory.

## 🖥️ Platform Support

- **Linux (X11)**: Full support with native libraries
- **macOS**: Full support via MacPorts
- **Windows**: Full support with pre-configured SDL2 libraries




## 🔗 Resources

- [SDL2 Documentation](https://wiki.libsdl.org/)
- [FreeImage Documentation](https://freeimage.sourceforge.io/)
- Project documentation: Build with `make doc` or `cmake --build . --target doc`

---

**Note**: This is a C-based GUI library project. Make sure you have a compatible C compiler (GCC, Clang, or MSVC) installed on your system.
