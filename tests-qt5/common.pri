LIBS        += -L$$OUT_PWD/$$DESTDIR/../lib -leventdispatcher_libev
INCLUDEPATH += $$PWD/../src
DEPENDPATH  += $$PWD/../src

unix:PRE_TARGETDEPS += $$DESTDIR/../lib/libeventdispatcher_libev.a

CONFIG += console link_prl
CONFIG -= app_bundle
