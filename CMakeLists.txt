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
)

add_library(grace STATIC
	src/pars.c
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

# Define the main executable
qt_standard_project_setup()

qt_add_executable(qtgrace
    src/MainWindow.ui
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
    grace
)

qt_add_resources(qtgrace "icons"
    PREFIX "/icons"
    BASE "fonts/icons"
    FILES
        fonts/icons/autoscale.png
        fonts/icons/BoxProps.png
        fonts/icons/ClearBoxes.png
        fonts/icons/ClearEllipses.png
        fonts/icons/ClearLines.png
        fonts/icons/ClearText.png
        fonts/icons/ClearText2.png
        fonts/icons/CloseObjects.png
        fonts/icons/CopyObject.png
        fonts/icons/CreateText.png
        fonts/icons/DeleteObject.png
        fonts/icons/DrawBox.png
        fonts/icons/DrawEllipse.png
        fonts/icons/DrawLine.png
        fonts/icons/EditObject.png
        fonts/icons/EllipseProps.png
        fonts/icons/goDown.png
        fonts/icons/goLeft.png
        fonts/icons/goRight.png
        fonts/icons/goUp.png
        fonts/icons/LineProps.png
        fonts/icons/MoveObject.png
        fonts/icons/Pan_Icon.png
        fonts/icons/Pick_Move_Icon.png
        fonts/icons/Redo_Icon.png
        fonts/icons/TextProps.png
        fonts/icons/Undo_Icon.png
        fonts/icons/zoomIn.png
        fonts/icons/zoomOut.png
        fonts/icons/zoom.png
        fonts/icons/settype_xy.svg
        fonts/icons/settype_xydx.svg
        fonts/icons/settype_xydy.svg
        fonts/icons/settype_xydxdx.svg
        fonts/icons/settype_xydydy.svg
        fonts/icons/settype_xydxdy.svg
        fonts/icons/settype_xydxdxdydy.svg
        fonts/icons/settype_bar.svg
        fonts/icons/settype_bardy.svg
        fonts/icons/settype_bardydy.svg
        fonts/icons/settype_xyhilo.svg
        fonts/icons/settype_xyz.svg
        fonts/icons/settype_xyr.svg
        fonts/icons/settype_xysize.svg
        fonts/icons/settype_xycolor.svg
        fonts/icons/settype_xycolpat.svg
        fonts/icons/settype_xyvmap.svg
        fonts/icons/settype_boxplot.svg
        fonts/icons/settype_band.svg
        fonts/icons/linetype_none.svg
        fonts/icons/linetype_straight.svg
        fonts/icons/linetype_lstairs.svg
        fonts/icons/linetype_rstairs.svg
        fonts/icons/linetype_segments.svg
        fonts/icons/linetype_3segments.svg
        fonts/icons/linetype_inc_x.svg
        fonts/icons/linetype_dec_x.svg
)

# Translations
set(TS_FILES qt_grace_ger.ts)

