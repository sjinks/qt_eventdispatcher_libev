LIBS           += -L$$PWD/../lib -leventdispatcher_libev
INCLUDEPATH    += $$PWD/../src
DEPENDPATH     += $$PWD/../src
PRE_TARGETDEPS += $$PWD/../lib/libeventdispatcher_libev.a
