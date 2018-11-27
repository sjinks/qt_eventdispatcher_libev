TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS  = src

greaterThan(QT_MAJOR_VERSION, 4) {
	SUBDIRS     += src-gui
	src-gui.file = src-gui/eventdispatcher_libev_qpa.pro
}

SUBDIRS += tests

src.file   = src/eventdispatcher_libev.pro
tests.file = tests/qt_eventdispatcher_tests/build.pro
