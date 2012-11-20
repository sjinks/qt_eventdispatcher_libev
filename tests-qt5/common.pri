LIBS           += -L$$PWD/../lib -leventdispatcher_libev
INCLUDEPATH    += $$PWD/../lib
DEPENDPATH     += $$PWD/../lib
PRE_TARGETDEPS += $$PWD/../lib/libeventdispatcher_libev.a
