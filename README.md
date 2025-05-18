# QtGrace6 - a port of QtGrace to QT6

[QtGrace](https://sourceforge.net/projects/qtgrace/) is a port of the [Grace](https://plasma-gate.weizmann.ac.il/Grace/) plotting tool to Qt5 developed by [Andreas Winter](https://sourceforge.net/u/andwin/profile/).

QtGrace project essencially resurrected amazing but severely outdated Grace software, but it has several important issues:
- It seems to be abandoned since 2023.
- It doesn't compile at all with modern GCC versions.
- It is hosted on SourceForge, which is objectively a very bad place for open source software distribution.
- It requires QtCreator to compile, which is an unnecessarily heavy dependency.
- It uses Qt5 and deprecated qmake build system instead of the standard CMake.

So, I decided to port the port and created QtGrace6 - a minor modification of original QtGrace, which now compiles with CMake, used Qt6 and is hosted on GitHub.

## Prerequsites

You need Qt6 with some optional modules. In Fedora linux it is enough to install the following:

`sudo dnf install qt6-qtbase-devel qt6-qtsvg-devel qt6-qt5compat-devel`

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