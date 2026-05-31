# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

QtGrace6 is a Qt6/CMake port of QtGrace (itself a Qt5 port of the classic Grace 2D plotting tool). It is a cross-platform GUI application for 2D data visualization and publication-quality plotting. The codebase is a mix of C (legacy Grace math/font libraries) and C++ (Qt6 application).

## Build

**Dependencies (Fedora):**
```bash
sudo dnf install cmake qt6-qtbase-devel qt6-qtsvg-devel
```

**Build:**
```bash
mkdir build && cd build
cmake ..
make
```

The executable can be run from anywhere — toolbar icons are compiled into the binary as Qt resources (`fonts/icons/icons.qrc`).

**Parallel build:**
```bash
make -j$(nproc)
```

## Architecture

The build produces three targets:

| Target | Type | Purpose |
|--------|------|---------|
| `libtype1.a` | static lib | PostScript Type1 font rendering (C, `type1/`) |
| `libgrace.a` | static lib | Cephes special math functions + T1 font tools (C, `src/*.c`) |
| `qtgrace` | executable | Qt6 GUI application (C++, `src/*.cpp`) |

`grace` links against `type1`; `qtgrace` links against Qt6 modules and `grace`.

### Qt Application Layer (`src/*.cpp`)

The application follows a largely monolithic structure inherited from the original Grace. Key files:

- **`fundamentals.cpp`** (~19k lines) — Core data structures: graphs, datasets, axes, annotations. This is the central state of the application.
- **`allWidgets.cpp`** (~30k lines) — The majority of dialog and widget implementations (graph properties, data transforms, fitting dialogs, etc.).
- **`editWidgets.cpp`** (~9k lines) — Editing-focused dialogs.
- **`appearanceWidgets.cpp`** (~7k lines) — Appearance/styling dialogs.
- **`MainWindow.cpp`** (~7k lines) — Main application window, menus, toolbar, and canvas.
- **`files.cpp`** (~9k lines) — File I/O: AGR project format, binary, CSV, ASCII imports.
- **`events.cpp`** (~4k lines) — Mouse and keyboard event handling on the canvas.
- **`draw.cpp`** + **`plotone.cpp`** + **`drawticks.cpp`** — Rendering pipeline: plot → axes/ticks → individual sets.
- **`graphs.cpp`** + **`graphutils.cpp`** + **`setutils.cpp`** — Graph and dataset management.

**Export drivers** (`*drv.cpp`): `pdfdrv.cpp`, `psdrv.cpp`, `svgdrv.cpp`, `gd.cpp` (bitmap formats), `mfdrv.cpp`, `mifdrv.cpp`, `rstdrv.cpp`, `dummydrv.cpp`.

**Data processing**: `fit.cpp`, `nonlfit.cpp` (curve fitting), `fourier.cpp` (FFT), `mathstuff.cpp`, `computils.cpp`.

**Other**: `undo_module.cpp` (undo/redo), `Server.cpp` (socket-based remote control), `compressdecompress.cpp` (zlib), `t1fonts.cpp` (Type1 font management).

### Platform Defines

CMake sets one of: `-DWINDOWS_SYSTEM`, `-DMAC_SYSTEM`, `-DLINUX_SYSTEM`. OpenGL is explicitly disabled (`-DQT_NO_OPENGL`).

### File Format

Project files use the `.agr` (Grace) format. Example files are in `examples/`. Templates are in `templates/`. The application always reads and writes **UTF-8**; legacy multi-encoding support has been removed.

### Icons

All toolbar icons are embedded as Qt resources (`fonts/icons/icons.qrc`, registered via `qt_add_resources` in `CMakeLists.txt`). They are accessed at runtime as `:/icons/<filename>.png`. The `fonts/icons/` directory is not needed at runtime.

### `-free` flag

Running `qtgrace -free <file>` enables free-layout mode: the plot resizes to fill the window as it is resized. Key implementation:
- `set_pagelayout(PAGE_FREE)` in `replacement_main.cpp` sets the mode.
- `MainArea::resizeEvent` (`MainWindow.cpp`) triggers a redraw on window resize.
- `sync_canvas_size` (`replacement_main.cpp`) tracks previous window dimensions in a static variable to compute incremental `rescale_viewport` deltas correctly — `device_table[DEVICE_SCREEN].pg` is restored to the original file dimensions after every draw, so it cannot be used as the "old" baseline.

## No Tests

There is no automated test suite. Manual testing uses the example files in `examples/*.agr`.

## Documentation

User-facing docs are in `doc/`: `UsersGuide.html`, `Tutorial.html`, `FAQ.html`, `CHANGES.html`. Grace command scripts (`.com` files) are in `scripts/`.
