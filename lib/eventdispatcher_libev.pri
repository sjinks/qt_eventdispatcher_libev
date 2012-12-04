HEADERS += $$PWD/eventdispatcher_libev.h

unix {
	CONFIG    += link_pkgconfig
	PKGCONFIG += eventdispatcher_libev
}
else:win32 {
	LIBS        += -L$$PWD -leventdispatcher_libev
	INCLUDEPATH += $$PWD
	DEPENDPATH  += $$PWD
}
