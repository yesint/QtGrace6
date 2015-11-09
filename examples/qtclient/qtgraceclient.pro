QT += network widgets

HEADERS       = client.h \
    communicationsProtocol.h
SOURCES       = client.cpp \
                main.cpp \
    communicationsProtocol.cc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/qtgraceclient
INSTALLS += target
