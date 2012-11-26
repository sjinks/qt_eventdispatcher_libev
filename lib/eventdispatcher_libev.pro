QT        -= gui
TARGET     = eventdispatcher_libev
TEMPLATE   = lib
CONFIG    += staticlib create_prl create_pc release
HEADERS   += eventdispatcher_libev.h eventdispatcher_libev_p.h
SOURCES   += eventdispatcher_libev.cpp eventdispatcher_libev_p.cpp timers_p.cpp socknot_p.cpp

unix {
	system('pkg-config --exists libev') {
		CONFIG    += link_pkgconfig
		PKGCONFIG += libev
	}
	else {
		LIBS += -lev
	}
}
else {
	LIBS += -lev
}

target.path   = /usr/lib
headers.path  = /usr/include
headers.files = eventdispatcher_libev.h

INSTALLS += target headers

QMAKE_PKGCONFIG_NAME        = eventdispatcher_libev
QMAKE_PKGCONFIG_DESCRIPTION = "LibEv-based event dispatcher for Qt"
QMAKE_PKGCONFIG_LIBDIR      = $$target.path
QMAKE_PKGCONFIG_INCDIR      = $$headers.path
QMAKE_PKGCONFIG_DESTDIR     = pkgconfig
