# Generated automatically from Makefile.in by configure.
# Makefile.in
# 
# This file is part of the t1lib-package! See documentation and Readme files
# of the t1lib-package for copyright restrictions.
#
# Targets provided: 
#                      all (default)
#                      install
#                      clean
#
# Author:              Rainer Menzner (Rainer.Menzner@web.de)
# Date:                06/20/1997
# Last modified:       2001-04-01
#
# Modified by H.Kakugawa to use libtool

LIBTOOL   = ../../libtool
SHELL     = /bin/sh
CC        = gcc
CPP       = gcc -E
CFLAGS    = -g -O2 -DT1LIB_IDENT="\"1.3.1\"" -DGLOBAL_CONFIG_DIR="\"${prefix}/share/t1lib\""
OPTIONS   = 
X_LIBS    =  -L/usr/X11R6/lib
TOPSRC    = ../..
XPM_LIB   = -lXpm
XLIB      =  -lSM -lICE -lXext -lX11 
LDFLAGS   = 
LDLIBS    = -lm
AR        = ar rc
RANLIB    = ranlib
RM        = rm -f
X_CFLAGS  =  -I/usr/X11R6/include
YACC      = @YACC@
LEX       = @LEX@
LEXLIB    = @LEXLIB@
ALLCFLAGS = $(CFLAGS) 

INSTALL         = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA    = ${INSTALL} -m 644
MKINSTALLDIRS   = ../../ac-tools/mkinstalldirs
prefix          = /usr/local
exec_prefix     = ${prefix}
includedir      = ${prefix}/include
bindir          = ${exec_prefix}/bin
datadir         = ${prefix}/share
libdir          = ${exec_prefix}/lib
mandir          = ${prefix}/man/man1
manext          = .1
no_x            = 

T1_AA_TYPE16       = -DT1_AA_TYPE16=short
T1_AA_TYPE32       = -DT1_AA_TYPE32=int
T1_AA_TYPE64       = 
SUBMAKE   = $(MAKE) 'CC=$(CC)' 'CFLAGS=$(ALLCFLAGS) $(T1_AA_TYPE16) $(T1_AA_TYPE32) $(T1_AA_TYPE64)' 'OPTIONS=$(OPTIONS)'




OBJS = \
	t1finfo.lo \
	t1base.lo \
	t1delete.lo \
	t1enc.lo \
	t1env.lo \
	t1load.lo \
	t1set.lo \
	t1trans.lo \
	t1aaset.lo \
	t1afmtool.lo \
	t1outline.lo \
	t1subset.lo \
	parseAFM.lo 

XOBJS = t1x11.lo

.SUFFIXES: .c .o .lo
.c.lo:
	$(LIBTOOL) --mode=compile \
		$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

.c.o:
	$(LIBTOOL) --mode=compile \
		$(CC) -c $(CPPFLAGS) $(CFLAGS) $< 

all: $(OBJS) $(XOBJS)

.PHONY: clean

dependencies: dummy 
	gcc -MM *.c | sed 's/\.o:/.lo:/g' > .dependencies 

clean: dummy
	-$(RM) *.lo *.o .libs/*
	-$(RM) *~ *.bak \#*\#
	-rmdir .libs


install: dummy


dummy:


# Dependencies of object files 
include .dependencies
