#-------------------------------------------------
#
# Project created by QtCreator 2020-01-05T21:05:56
#
#-------------------------------------------------

QT       -= core gui

TARGET = hpdf
TEMPLATE = lib
CONFIG += staticlib

CONFIG -= debug \
    debug_and_release
CONFIG += release

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += libhpdf.cpp \
    hpdf_3dmeasure.c \
    hpdf_annotation.c \
    hpdf_array.c \
    hpdf_binary.c \
    hpdf_boolean.c \
    hpdf_catalog.c \
    hpdf_destination.c \
    hpdf_dict.c \
    hpdf_doc.c \
    hpdf_doc_png.c \
    hpdf_encoder.c \
    hpdf_encoder_cns.c \
    hpdf_encoder_cnt.c \
    hpdf_encoder_jp.c \
    hpdf_encoder_kr.c \
    hpdf_encoder_utf.c \
    hpdf_encrypt.c \
    hpdf_encryptdict.c \
    hpdf_error.c \
    hpdf_exdata.c \
    hpdf_ext_gstate.c \
    hpdf_font.c \
    hpdf_font_cid.c \
    hpdf_font_tt.c \
    hpdf_font_type1.c \
    hpdf_fontdef.c \
    hpdf_fontdef_base14.c \
    hpdf_fontdef_cid.c \
    hpdf_fontdef_cns.c \
    hpdf_fontdef_cnt.c \
    hpdf_fontdef_jp.c \
    hpdf_fontdef_kr.c \
    hpdf_fontdef_tt.c \
    hpdf_fontdef_type1.c \
    hpdf_gstate.c \
    hpdf_image.c \
    hpdf_image_ccitt.c \
    hpdf_image_png.c \
    hpdf_info.c \
    hpdf_list.c \
    hpdf_mmgr.c \
    hpdf_name.c \
    hpdf_namedict.c \
    hpdf_null.c \
    hpdf_number.c \
    hpdf_objects.c \
    hpdf_outline.c \
    hpdf_page_label.c \
    hpdf_page_operator.c \
    hpdf_pages.c \
    hpdf_pdfa.c \
    hpdf_real.c \
    hpdf_streams.c \
    hpdf_string.c \
    hpdf_u3d.c \
    hpdf_utils.c \
    hpdf_xref.c

HEADERS += libhpdf.h \
    hpdf.h \
    hpdf_3dmeasure.h \
    hpdf_annotation.h \
    hpdf_catalog.h \
    hpdf_conf.h \
    hpdf_consts.h \
    hpdf_destination.h \
    hpdf_doc.h \
    hpdf_encoder.h \
    hpdf_encrypt.h \
    hpdf_encryptdict.h \
    hpdf_error.h \
    hpdf_exdata.h \
    hpdf_ext_gstate.h \
    hpdf_font.h \
    hpdf_fontdef.h \
    hpdf_gstate.h \
    hpdf_image.h \
    hpdf_info.h \
    hpdf_list.h \
    hpdf_mmgr.h \
    hpdf_namedict.h \
    hpdf_objects.h \
    hpdf_outline.h \
    hpdf_page_label.h \
    hpdf_pages.h \
    hpdf_pdfa.h \
    hpdf_streams.h \
    hpdf_types.h \
    hpdf_u3d.h \
    hpdf_utils.h \
    hpdf_version.h \
    t4.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
