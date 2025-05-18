# Target ../bin/qtgrace
# For Windows Systems
!unix:DEFINES += WINDOWS_SYSTEM

# For Mac OS X Systems
macx:DEFINES += MAC_SYSTEM

# For Linux Systems
unix:!macx:DEFINES += LINUX_SYSTEM

TRANSLATIONS += qt_grace_ger.ts
QT += network
QT += svg
QT -= opengl
greaterThan(QT_MAJOR_VERSION,5): QT += core5compat
DEFINES += QT_NO_OPENGL

greaterThan(QT_MAJOR_VERSION, 4) {
QT += widgets
QT += printsupport
}

#greaterThan(QT_MAJOR_VERSION, 5) {
#QT += qt5support
#}

ICON = GraceIcon.icns
!unix:RC_FILE = qtgrace.rc

# Activate the following options if you have the static libraries for fftw3 and libHaru (otherwise dynamic libraries are to be used)
#DEFINES += USE_STATIC_EXT_LIB_FFTW3
#unix:LIBS += -lfftw3
#win32:LIBS += "C:\path\to\libfftw3.a"

#DEFINES += USE_STATIC_EXT_LIB_HARU
#unix:LIBS += -lhpdf
#win32:LIBS += "C:\path\to\libhpdf.a"
#win32:LIBS += -lz

win32 {
!contains(QMAKE_CC, gcc) {
QMAKE_CXXFLAGS += /showIncludes
QMAKE_CXXFLAGS_RELEASE += /showIncludes
QMAKE_CXXFLAGS -= -Zc:strictStrings
QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings
}
}

CONFIG -= debug \
    debug_and_release
CONFIG += release


#QMAKE_CFLAGS +=-Werror=format-security
#QMAKE_CXXFLAGS +=-Werror=format-security
#QMAKE_CXXFLAGS -= -O2
#QMAKE_CFLAGS -= -O2
#QMAKE_CXXFLAGS += -O3
#QMAKE_CFLAGS += -O3
#QMAKE_CXXFLAGS += -Wall
#QMAKE_CFLAGS += -Wall
#QMAKE_CXXFLAGS += -Wuninitialized
#QMAKE_CFLAGS += -Wuninitialized

HEADERS += MainWindow.h \
    allWidgets.h \
    appearanceWidgets.h \
    compressdecompress.h \
    editWidgets.h \
    graphs.h \
    defines.h \
    noxprotos.h \
    utils.h \
    globals.h \
    parser.h \
    draw.h \
    missing.h \
    graphutils.h \
    device.h \
    t1fonts.h \
    plotone.h \
    ssdata.h \
    files.h \
    cmath.h \
    mathstuff.h \
    cephes.h \
    mconf.h \
    dlmodule.h \
    as274c.h \
    f2c.h \
    devlist.h \
    dummydrv.h \
    mfdrv.h \
    mifdrv.h \
    pdfdrv.h \
    psdrv.h \
    rstdrv.h \
    svgdrv.h \
    patterns.h \
    gd.h \
    t1lib.h \
    windowWidgets.h \
    x11drv.h \
    xprotos.h \
    bitmaps.h \
    jbitmaps.h \
    mbitmaps.h \
    events.h \
    fundamentals.h \
    t1set.h \
    t1aaset.h \
    parseAFM.h \
    t1extern.h \
    t1types.h \
    t1load.h \
    t1misc.h \
    t1base.h \
    t1finfo.h \
    t1outline.h \
    t1afmtool.h \
    t1enc.h \
    t1env.h \
    sysconf.h \
    t1delete.h \
    t1global.h \
    config.h \
    t1trans.h \
    ../type1/arith.h \
    ../type1/blues.h \
    ../type1/cluts.h \
    ../type1/curves.h \
    ../type1/digit.h \
    ../type1/ffilest.h \
    ../type1/fontfcn.h \
    ../type1/fontfile.h \
    ../type1/font.h \
    ../type1/fontmisc.h \
    ../type1/fonts.h \
    ../type1/fontstruct.h \
    ../type1/fontxlfd.h \
    ../type1/fsmasks.h \
    ../type1/hdigit.h \
    ../type1/hints.h \
    ../type1/lines.h \
    ../type1/objects.h \
    ../type1/paths.h \
    ../type1/paths_rmz.h \
    ../type1/pictures.h \
    ../type1/regions.h \
    ../type1/spaces.h \
    ../type1/spaces_rmz.h \
    ../type1/strokes.h \
    ../type1/t1hdigit.h \
    ../type1/t1imager.h \
    ../type1/t1intf.h \
    ../type1/t1stdio.h \
    ../type1/token.h \
    ../type1/tokst.h \
    ../type1/trig.h \
    ../type1/types.h \
    ../type1/util.h \
    ../type1/Xstuff.h \
    Server.h \
    undo_module.h \
    include/hpdf.h \
    fftw3.h \
    external_libs.h \
    KeyAndMousePressFilter.h
SOURCES += allWidgets.cpp \
    appearanceWidgets.cpp \
    compressdecompress.cpp \
    editWidgets.cpp \
    qtgrace.cpp \
    MainWindow.cpp \
    graphs.cpp \
    drawticks.cpp \
    draw.cpp \
    objutils.cpp \
    setutils.cpp \
    dates.cpp \
    defaults.cpp \
    graphutils.cpp \
    device.cpp \
    t1fonts.cpp \
    regionutils.cpp \
    plotone.cpp \
    params.cpp \
    ssdata.cpp \
    files.cpp \
    dlmodule.cpp \
    pars.c \
    utils.cpp \
    as274c.cpp \
    fit.cpp \
    nonlfit.cpp \
    mathstuff.cpp \
    computils.cpp \
    humlik.cpp \
    fourier.cpp \
    lmdif.cpp \
    iofilters.cpp \
    replacement_main.cpp \
    dummydrv.cpp \
    mfdrv.cpp \
    mifdrv.cpp \
    pdfdrv.cpp \
    psdrv.cpp \
    rstdrv.cpp \
    svgdrv.cpp \
    gd.cpp \
    windowWidgets.cpp \
    xutil.cpp \
    x11drv.cpp \
    events.cpp \
    fundamentals.cpp \
    t1set.c \
    t1aaset.c \
    parseAFM.c \
    t1load.c \
    t1base.c \
    t1finfo.c \
    t1outline.c \
    t1afmtool.c \
    t1enc.c \
    t1env.c \
    t1delete.c \
    t1trans.c \
    ../type1/arith.c \
    ../type1/bstring.c \
    ../type1/curves.c \
    ../type1/fontfcn.c \
    ../type1/hints.c \
    ../type1/lines.c \
    ../type1/objects.c \
    ../type1/paths.c \
    ../type1/regions.c \
    ../type1/scanfont.c \
    ../type1/spaces.c \
    ../type1/t1io.c \
    ../type1/t1snap.c \
    ../type1/t1stub.c \
    ../type1/token.c \
    ../type1/type1.c \
    ../type1/util.c \
    acosh.c \
    airy.c \
    asinh.c \
    atanh.c \
    beta.c \
    cbrt.c \
    chbevl.c \
    chdtr.c \
    const.c \
    dawsn.c \
    ellie.c \
    ellik.c \
    ellpe.c \
    ellpk.c \
    expn.c \
    fac.c \
    fdtr.c \
    fresnl.c \
    gamma.c \
    gdtr.c \
    hyp2f1.c \
    hyperg.c \
    i0.c \
    i1.c \
    igam.c \
    igami.c \
    incbet.c \
    incbi.c \
    iv.c \
    j0.c \
    j1.c \
    jn.c \
    jv.c \
    k0.c \
    k1.c \
    kn.c \
    mtherr.c \
    ndtr.c \
    ndtri.c \
    pdtr.c \
    polevl.c \
    polyn.c \
    psi.c \
    revers.c \
    rgamma.c \
    round.c \
    shichi.c \
    sici.c \
    spence.c \
    stdtr.c \
    struve.c \
    unity.c \
    yn.c \
    zeta.c \
    zetac.c \
    undo_module.cpp \
    Server.cpp \
    ListOfLatexCommands.cpp \
    external_libs.cpp \
    KeyAndMousePressFilter.cpp
TARGET = ../bin/qtgrace
