# QtGrace6 - a port of Grace to QT6

## Motivation
[Grace](https://plasma-gate.weizmann.ac.il/Grace/) is an amazing plotting tool for  Unix-like operating systems, which remains unbeatable for quick visualization of data and making publication-quality 2D plots combining rich command-line interface and visual editing. However, its GUI is showing the age and doesn't work on Windows and MacOS.

In 2011 [Andreas Winter](https://sourceforge.net/u/andwin/profile/) ported Grace to Qt5 and created [QtGrace](https://sourceforge.net/projects/qtgrace/), which works on all major platforms and adds modern slick interface and multiple new features. Unfortunately original QtGrace has several important issues:
- It seems to be abandoned since 2023.
- It stopped compiling on GCC 15 and higher due to enforcement of stricter C23 standard.
- It is hosted on SourceForge, which is objectively a very bad place for open source software distribution.
- It requires QtCreator to compile, which is a huge overkill as a dependency and complicates distribution and packaging a lot.
- It only compiles with deprecated Qt5 and qmake build system instead of the standard CMake.

So, I decided to port the port and created QtGrace6 - a pretty minor modification of original QtGrace, which compiles with CMake and Qt6 and is hosted on GitHub.

## Prerequsites

You need modern C++ compiler, cmake and Qt6 with optional `qt5compat` and `qtsvg` modules. On Fedora linux it is enough to install the following:

`sudo dnf install cmake qt6-qtbase-devel qt6-qtsvg-devel qt6-qt5compat-devel`

On other distributions and platforms you need to google how these packages are called.

## Installation

In the source directory do the following:

```
mkdir build && cd build
cmake .. 
make
```

Resulting executable should stay in the `build` directory because otherwise it doesn't see required font files, so the best way is to create an alias to it in your shell profile.

## Disclaimer

Currently QtGrace6 is only tested on Fedora Linux. I'm not interested in debugging the build on other platforms, so please test it yourself and sumbit pull requests if it works.