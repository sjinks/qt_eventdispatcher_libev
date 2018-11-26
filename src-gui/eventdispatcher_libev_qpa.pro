QT      += gui-private
CONFIG  += staticlib create_prl link_prl
TEMPLATE = lib
TARGET   = eventdispatcher_libev_qpa
SOURCES  = eventdispatcher_libev_qpa.cpp
HEADERS  = eventdispatcher_libev_qpa.h
DESTDIR  = ../lib

LIBS           += -L$$PWD/../lib -leventdispatcher_libev
INCLUDEPATH    += $$PWD/../src
DEPENDPATH     += $$PWD/../src
PRE_TARGETDEPS += $$DESTDIR/../lib/libeventdispatcher_libev.a
