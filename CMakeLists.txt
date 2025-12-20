cmake_minimum_required(VERSION 3.10)
project(qtgrace6 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# OS‐specific DEFINES
if(WIN32)
    add_definitions(-DWINDOWS_SYSTEM)
elseif(APPLE)
    add_definitions(-DMAC_SYSTEM)
elseif(UNIX)
    add_definitions(-DLINUX_SYSTEM)
endif()

# Disable OpenGL
add_definitions(-DQT_NO_OPENGL)

# Icon and resource files
if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE GraceIcon.icns)
    set_source_files_properties(${CMAKE_SOURCE_DIR}/GraceIcon.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
elseif(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS qtgrace.rc)
endif()

# Compiler flags (Windows-specific)
if(MSVC)
    add_compile_options(/showIncludes)
endif()

# Find the same Qt modules your .pro does
find_package(Qt6 REQUIRED COMPONENTS
    Network
    Svg
    Core
    Widgets
    PrintSupport
    Core5Compat
)

add_library(grace STATIC
	src/pars.c
	src/t1set.c
	src/t1aaset.c
	src/parseAFM.c
	src/t1load.c
	src/t1base.c
	src/t1finfo.c
	src/t1outline.c
	src/t1afmtool.c
	src/t1enc.c
	src/t1env.c
	src/t1delete.c
	src/t1trans.c
	src/acosh.c
	src/airy.c
	src/asinh.c
	src/atanh.c
	src/beta.c
	src/cbrt.c
	src/chbevl.c
	src/chdtr.c
	src/const.c
	src/dawsn.c
	src/ellie.c
	src/ellik.c
	src/ellpe.c
	src/ellpk.c
	src/expn.c
	src/fac.c
	src/fdtr.c
	src/fresnl.c
	src/gamma.c
	src/gdtr.c
	src/hyp2f1.c
	src/hyperg.c
	src/i0.c
	src/i1.c
	src/igam.c
	src/igami.c
	src/incbet.c
	src/incbi.c
	src/iv.c
	src/j0.c
	src/j1.c
	src/jn.c
	src/jv.c
	src/k0.c
	src/k1.c
	src/kn.c
	src/mtherr.c
	src/ndtr.c
	src/ndtri.c
	src/pdtr.c
	src/polevl.c
	src/polyn.c
	src/psi.c
	src/revers.c
	src/rgamma.c
	src/round.c
	src/shichi.c
	src/sici.c
	src/spence.c
	src/stdtr.c
	src/struve.c
	src/unity.c
	src/yn.c
	src/zeta.c
	src/zetac.c
)
target_include_directories(grace PRIVATE ./src)
target_link_libraries(grace PRIVATE type1)

add_library(type1 STATIC
	type1/arith.c
	type1/bstring.c
	type1/curves.c
	type1/fontfcn.c
	type1/hints.c
	type1/lines.c
	type1/objects.c
	type1/paths.c
	type1/regions.c
	type1/scanfont.c
	type1/spaces.c
	type1/t1io.c
	type1/t1snap.c
	type1/t1stub.c
	type1/token.c
	type1/type1.c
	type1/util.c
)
target_include_directories(type1 PRIVATE ./type1)

# Define the main executable
qt_standard_project_setup()

qt_add_executable(qtgrace
    src/allWidgets.cpp
    src/appearanceWidgets.cpp
    src/compressdecompress.cpp
    src/editWidgets.cpp
    src/qtgrace.cpp
    src/MainWindow.cpp
    src/graphs.cpp
    src/drawticks.cpp
    src/draw.cpp
    src/objutils.cpp
    src/setutils.cpp
    src/dates.cpp
    src/defaults.cpp
    src/graphutils.cpp
    src/device.cpp
    src/t1fonts.cpp
    src/regionutils.cpp
    src/plotone.cpp
    src/params.cpp
    src/ssdata.cpp
    src/files.cpp
    src/dlmodule.cpp
    src/utils.cpp
    src/as274c.cpp
    src/fit.cpp
    src/nonlfit.cpp
    src/mathstuff.cpp
    src/computils.cpp
    src/humlik.cpp
    src/fourier.cpp
    src/lmdif.cpp
    src/iofilters.cpp
    src/replacement_main.cpp
    src/dummydrv.cpp
    src/mfdrv.cpp
    src/mifdrv.cpp
    src/pdfdrv.cpp
    src/psdrv.cpp
    src/rstdrv.cpp
    src/svgdrv.cpp
    src/gd.cpp
    src/windowWidgets.cpp
    src/xutil.cpp
    src/x11drv.cpp
    src/events.cpp
    src/fundamentals.cpp
    src/undo_module.cpp
    src/Server.cpp
    src/ListOfLatexCommands.cpp
    src/external_libs.cpp
    src/KeyAndMousePressFilter.cpp
    src/qtgrace.rc
)
target_include_directories(qtgrace PRIVATE ./src)

# Windows icon resource
if(WIN32 AND EXISTS ${APP_ICON_RESOURCE_WINDOWS})
    target_sources(qtgrace PRIVATE ${APP_ICON_RESOURCE_WINDOWS})
endif()

# Link libraries
target_link_libraries(qtgrace PRIVATE
    Qt6::Core
    Qt6::Network
    Qt6::Svg
    Qt6::Widgets
    Qt6::PrintSupport
    Qt6::Core5Compat
    grace
)

# Translations
set(TS_FILES qt_grace_ger.ts)

