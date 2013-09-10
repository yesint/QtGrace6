
include $(MAKEFILEHEADHOME)/Makefile.head
GUI=GUI
UP=../../../src

CFILES=acosh.c cbrt.c ellie.c fac.c hyp2f1.c igami.c j0.c k1.c ndtri.c polyn.c shichi.c t1aaset.c t1env.c t1trans.c zeta.c\
 airy.c chbevl.c ellik.c fdtr.c hyperg.c incbet.c j1.c kn.c pars.c psi.c sici.c t1afmtool.c t1finfo.c unity.c zetac.c\
 asinh.c chdtr.c ellpe.c fresnl.c i0.c incbi.c jn.c log2.c parseAFM.c revers.c spence.c t1base.c t1load.c \
 atanh.c const.c ellpk.c gamma.c i1.c isnan.c jv.c mtherr.c pdtr.c rgamma.c stdtr.c t1delete.c t1outline.c yn.c\
 beta.c dawsn.c expn.c gdtr.c igam.c iv.c k0.c ndtr.c polevl.c round.c struve.c t1enc.c t1set.c

T1NAMES=\
    arith.c \
    curves.c \
    fontfcn.c \
    hints.c \
    lines.c \
    objects.c \
    paths.c \
    regions.c \
    scanfont.c \
    spaces.c \
    t1io.c \
    t1snap.c \
    t1stub.c \
    token.c \
    type1.c \
    util.c

# Removed bstring.h, it contains just   'int memset(void *,int,int)'

T1CFILES=$(T1NAMES:%.c=..\type1\%.c)

CPPFILES=device.cpp  graphs.cpp plotone.cpp utils.cpp\
 as274c.cpp dlmodule.cpp files.cpp graphutils.cpp mathstuff.cpp psdrv.cpp setutils.cpp x11drv.cpp\
 check_float.cpp draw.cpp fit.cpp humlik.cpp mfdrv.cpp nonlfit.cpp qtgrace.cpp ssdata.cpp xutil.cpp\
 computils.cpp drawticks.cpp fourier.cpp iofilters.cpp mifdrv.cpp objutils.cpp regionutils.cpp svgdrv.cpp \
 dates.cpp dummydrv.cpp ListOfLatexCommands.cpp params.cpp replacement_main.cpp t1fonts.cpp\
 defaults.cpp events.cpp gd.cpp lmdif.cpp pdfdrv.cpp rstdrv.cpp undo_module.cpp rint.cpp

TOMOCFILES = allWidgets.cpp fundamentals.cpp MainWindow.cpp Server.cpp

MOCFILES=$(TOMOCFILES:%.cpp=moc_%.cpp)

MOCOBJFILES=$(MOCFILES:%.cpp=%.o)		
COBJFILES=$(CFILES:%.c=%.o)
CPPOBJFILES=$(CPPFILES:%.cpp=%.o)
TOMOCOBJFILES=$(TOMOCFILES:%.cpp=%.o)
T1OBJFILES=$(T1NAMES:%.c=%.o)

OBJS=$(MOCOBJFILES) $(COBJFILES) $(CPPOBJFILES) $(TOMOCOBJFILES) $(T1OBJFILES)
ABIOBJS= $(OBJS:%.o=$(ABI)/%.o)
SRC=$(CFILES) $(CPPFILES) $(TOMOCFILES) $(T1CFILES)

B_INCLUDES= 

INCLUDES= -I.	$(B_INCLUDES) $(QTINCLUDE)




LIBS = 	-L$(XHOME)/lib64 \
	-L$(XHOME)/lib \
        -lm \
        -L$(QT4HOME)/lib/$(ABI) $(QT4LIBS)  $(XTRLIBS) 
#INCLUDES += -I$(QT4HOME)/include/QtWebKit

ifneq (,$(findstring -DQT5,$(commdef)))
#INCLUDES += -I$(QT4HOME)/include/QtWebKitWidgets
endif

ifneq (,$(findstring LINUX,$(ABI)))
CCFLAGS+=-DLINUX_SYSTEM
#LIBS+=-lQtWebKit
else
CCFLAGS+=-DWINDOWS_SYSTEM
endif

BIN=qtGrace$(ABI)

default: 
	$(MAKE) dir
	$(MAKE) depend 
	$(MAKE) $(BIN)

info:
	echo  TOMOCFILES $(TOMOCFILES)
	echo  MOCFILES  $(MOCFILES)
	echo  MOCOBJFILES $(MOCOBJFILES)
	echo OBJS $(OBJS)



$(BIN): $(ABIOBJS) 
	$(LINK)  -o $(BIN)  $(OPTFLAGS4)  $(ABIOBJS) $(LIBS)

$(ABI)/%.o: %.cpp
	$(CC) $(DEFINES) $(CCFLAGS) -U COMPLEX $(OPTFLAGS4) $(INCLUDES) -c $< -o $@

$(ABI)/%.o: ../type1/%.c
	$(cc) $(DEFINES) $(CCFLAGS) -U COMPLEX $(OPTFLAGS4) $(INCLUDES) -c $< -o $@

$(ABI)/%.o: %.c
	$(cc) $(DEFINES) $(CCFLAGS) -U COMPLEX $(OPTFLAGS4) $(INCLUDES) -c $< -o $@

moc_%.cpp: %.h
	$(MOC4) $< -o $@

.SECONDARY: $(MSRC)


all: default 

clean:
	rm -f $(ABI)/*.o $(BIN)   core  

dir:
	@ ( if [ ! -d $(ABI) ] ; then mkdir $(ABI) ; fi)

BINABIDIR=$(BEAST)/bin/$(ABI)

install: $(BIN)
	if [ ! -d $(BINABIDIR)/qtgrace ] ; then mkdir -p $(BINABIDIR)/qtgrace ; fi
	if [ -f $(BIN).manifest ] ; then \
		(mt.exe -manifest $(BIN).manifest -outputresource:$(BIN)\;1);\
		echo "Manifsetr done";\
		fi
	if [ -f  $(BINABIDIR)/qtgrace/bin/qtgrace$(FEXT) ] ; then rm $(BINABIDIR)/qtgrace/bin/qtgrace$(FEXT); fi
	cp -r ../fonts ../bin ../doc ../examples $(BINABIDIR)/qtgrace
	cp $(BIN) $(BINABIDIR)/qtgrace/bin/qtgrace$(FEXT)


$(ABI)/makedepend.tail:
	if [ ! -d $(ABI) ] ; then mkdir -p $(ABI) ; fi ;
	touch $(ABI)/makedepend.tail

#pars.c:pars.yacc
#	bison 
# Note: kdiff3 y.tab.c pars.c shows that pars.yacc was used for generating
#  y.tab.c, but in 2008 this file was manually modified to pars.c.
# How to fix ?

.FORCE:
	@echo FORCE

# BZ342
# Enforce recompilation of MOC files when dependent headers change, like this:
# WINDOWS32/moc_QtBeastFileSelect.o: WINDOWS32/QtBeastFileSelect.o

depend: 
	@echo > $(ABI)/makedepend.tail
	makedepend -Y -p"\$$(ABI)/" -f $(ABI)/makedepend.tail \
			$(B_INCLUDES) $(SRC) $(FORDEPEND) 2> /dev/null
	for i in ${TOMOBJS} ; do \
		echo  "\$$(ABI)/moc_"$$i: "\$$(ABI)/"$$i  >> $(ABI)/makedepend.tail ;\
	done



# if this file doesn't exist, a touch may be enough.
include $(ABI)/makedepend.tail
